// ================= LOAD HTML INTO ELEMENT =================
function loadHTML(elementId, file) {
    fetch(file)
    .then(response => {
        if (!response.ok) {
            throw new Error("Failed to load " + file);
        }
        return response.text();
    })
    .then(data => {
        document.getElementById(elementId).innerHTML = data;
    })
    .catch(error => {
        console.error(error);
        document.getElementById(elementId).innerHTML =
            "<p style='color:red;'>Error loading " + file + "</p>";
    });
}

// ================= LOAD MAIN CONTENT (IFRAME) =================
function loadMain(file) {
    const main = document.getElementById("mainContent");

    main.innerHTML = `
        <iframe src="${file}" onload="scrollToTop()"></iframe>
    `;
}

// ================= SCROLL FIX =================
function scrollToTop() {
    window.scrollTo({
        top: 0,
        behavior: "smooth"
    });
}

// ================= INITIAL LOAD =================
function loadAll() {

    // Load static parts
    loadHTML("header", "Assets/header.htm");
    loadHTML("nav", "Assets/Navigation.htm");
    loadHTML("footer", "Assets/footer.htm");

    // Default page
    loadMain("Assets/main.htm");
}