//import * as showdown from "https://cdn.jsdelivr.net/npm/showdown@2.1.0/dist/showdown.min.js"
import { marked } from "https://cdn.jsdelivr.net/npm/marked/lib/marked.esm.js";

var markdownPreview = document.getElementById("markdown_preview")
var markdownContent = document.getElementById("markdown_content")
// var converter = new showdown.converter()

if (markdownContent != null)
{
    markdownContent.addEventListener("keyup", function(_) {
        if (markdownPreview != null) {
            markdownPreview.innerHTML = marked.parse(markdownContent.innerText)
        }
    })

    markdownContent.dispatchEvent(new KeyboardEvent("keyup"))
}
