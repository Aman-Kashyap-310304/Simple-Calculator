// ================= LOAD HTML INTO ELEMENT =================
function loadHTML(elementId, file) {
    return fetch(file)
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

// ================= LOAD MAIN CONTENT =================
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

// ================= LOAD ANIMATION OVERLAY =================
function loadAnimation() {
    fetch("Assets/Animation.htm")
    .then(res => res.text())
    .then(data => {
        const overlay = document.createElement("div");
        overlay.id = "animationOverlay";
        overlay.innerHTML = data;
        document.body.appendChild(overlay);
    });
}

// ================= HIDE ANIMATION =================
function hideAnimation() {
    const overlay = document.getElementById("animationOverlay");

    if (overlay) {
        overlay.style.transition = "opacity 0.5s ease";
        overlay.style.opacity = "0";

        setTimeout(() => {
            overlay.remove();
        }, 500);
    }
}

// ================= RESOURCE PRELOADER =================
function resourceLoader() {

    const resources = [
        "Assets/header.htm",
        "Assets/Navigation.htm",
        "Assets/footer.htm",
        "Assets/main.htm",
        "Assets/Calculator.htm"
    ];

    return Promise.all(
        resources.map(file =>
            fetch(file).catch(err => console.error("Preload failed:", file))
        )
    );
}

// ================= INITIAL LOAD =================
function loadAll() {

    // STEP 1: SHOW ANIMATION
    loadAnimation();

    // STEP 2: PRELOAD RESOURCES
    resourceLoader()
    .then(() => {

        // STEP 3: LOAD ACTUAL CONTENT
        return Promise.all([
            loadHTML("header", "Assets/header.htm"),
            loadHTML("nav", "Assets/Navigation.htm"),
            loadHTML("footer", "Assets/footer.htm")
        ]);
    })
    .then(() => {

        loadMain("Assets/main.htm");

        // STEP 4: HIDE ANIMATION AFTER 5 SEC
        setTimeout(() => {
            hideAnimation();
        }, 5000);
    });
}
