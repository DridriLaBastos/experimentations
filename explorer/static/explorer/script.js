const csrf_token = document.querySelector("meta[name=csrf-token]").getAttribute("content")

document.getElementById("disconnect-button").addEventListener("click", function() {
    fetch("/connection/disconnect", {
        method: "POST",
        headers: {"X-CSRFToken": csrf_token},
    })
})