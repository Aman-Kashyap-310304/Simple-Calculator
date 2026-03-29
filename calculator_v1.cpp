#include <graphics.h>
#include <winbgim.h>
#include <conio.h>
#include <windows.h>
#include <stack>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

using namespace std;

// ================= GLOBAL =================
string input = "";
string displayLine = "";
bool newInput = false;

// ================= BUTTON =================
struct Button {
    int x, y, w, h;
    string label;
};
vector<Button> btn;

// ================= FORMAT =================
string format(double val) {
    if (floor(val) == val)
        return to_string((long long)val);

    ostringstream ss;
    ss << val;
    return ss.str();
}

// ================= PARSER =================
int prec(char op) {
    if (op=='+'||op=='-') return 1;
    if (op=='*'||op=='/') return 2;
    return 0;
}

double apply(double a,double b,char op) {
    if(op=='+') return a+b;
    if(op=='-') return a-b;
    if(op=='*') return a*b;
    if(op=='/') return (b!=0)?a/b:0;
    return 0;
}

double evaluate(string s) {

    stack<double> val;
    stack<char> op;

    for(int i=0;i<s.length();i++) {

        if(isdigit(s[i])||s[i]=='.') {
            string num;
            while(i<s.length()&&(isdigit(s[i])||s[i]=='.'))
                num+=s[i++];
            i--;
            val.push(stod(num));
        }
        else {

            while(!op.empty() && val.size()>=2 &&
                  prec(op.top())>=prec(s[i])) {

                double b=val.top(); val.pop();
                double a=val.top(); val.pop();

                char o=op.top(); op.pop();
                val.push(apply(a,b,o));
            }
            op.push(s[i]);
        }
    }

    while(!op.empty() && val.size()>=2) {
        double b=val.top(); val.pop();
        double a=val.top(); val.pop();

        char o=op.top(); op.pop();
        val.push(apply(a,b,o));
    }

    return val.empty()?0:val.top();
}

// ================= INIT =================
void initButtons() {

    int x=40,y=180,w=70,h=70;

    string labels[]={
        "7","8","9","/",
        "4","5","6","*",
        "1","2","3","-",
        "0",".","=","+"
    };

    int k=0;
    for(int r=0;r<4;r++) {
        for(int c=0;c<4;c++) {
            btn.push_back({x+c*w,y+r*h,w,h,labels[k++]});
        }
    }
}

// ================= DRAW =================
void drawUI() {

    setbkcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    bar(0,0,getmaxx(),getmaxy());

    setcolor(BLACK);

    // INPUT LINE
    settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
    outtextxy(40,110,(char*)input.c_str());

    // RESULT LINE
    int screenWidth = getmaxx();
    outtextxy(screenWidth - textwidth((char*)displayLine.c_str()) - 20,
          140,
          (char*)displayLine.c_str());

    // CLEAR
    setfillstyle(SOLID_FILL,LIGHTRED);
    bar(40,40,120,80);
    setcolor(BLACK);
    outtextxy(70,55,"C");

    // BUTTONS
    for(auto &b:btn) {

        bool op=(b.label=="+"||b.label=="-"||b.label=="*"||b.label=="/"||b.label=="=");

        setfillstyle(SOLID_FILL,op?COLOR(255,140,0):LIGHTGRAY);
        bar(b.x,b.y,b.x+b.w,b.y+b.h);

        setcolor(BLACK);
        rectangle(b.x,b.y,b.x+b.w,b.y+b.h);

        outtextxy(b.x+28,b.y+25,(char*)b.label.c_str());
    }
}

// ================= CLICK =================
void handleClick(int mx,int my) {

    // CLEAR
    if(mx>=40&&mx<=120&&my>=40&&my<=80) {
        input="";
        displayLine="";
        return;
    }

    for(auto &b:btn) {

        if(mx>=b.x&&mx<=b.x+b.w &&
           my>=b.y&&my<=b.y+b.h) {

            string val=b.label;

            // NUMBER / DOT
            if(isdigit(val[0]) || val==".") {

                if(newInput) {
                    input="";
                    newInput=false;
                }

                // prevent multiple dots in number
                int i=input.length()-1;
                while(i>=0 && (isdigit(input[i])||input[i]=='.')) {
                    if(input[i]=='.' && val==".") return;
                    i--;
                }

                input+=val;
            }

            // OPERATOR
            else if(val=="+"||val=="-"||val=="*"||val=="/") {

                if(input.empty()) return;

                // prevent double operator
                if(!isdigit(input.back()) && input.back()!='.')
                    input.back()=val[0];
                else
                    input+=val;

                newInput=false;
            }

            // EQUAL
            else if(val=="=") {

                if(input.empty()) return;

                double result=evaluate(input);

                displayLine = input + " = " + format(result);

                input = format(result);
                newInput = true;
            }
        }
    }
}

// ================= MAIN =================
int main() {

    initwindow(400,560,"Calculator - by Aman Kashyap DCE 27921");

    initButtons();

    int page=0;

    while(true) {

        setactivepage(page);
        setvisualpage(1-page);

        drawUI();
        page=1-page;

        if(ismouseclick(WM_LBUTTONDOWN)) {
            int x,y;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            handleClick(x,y);
        }

        if(kbhit() && getch()==27) break;

        delay(10);
    }

    closegraph();
    return 0;
}