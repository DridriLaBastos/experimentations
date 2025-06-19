import { marked } from "https://cdn.jsdelivr.net/npm/marked/lib/marked.esm.js";

var markdownPreview = document.getElementById("markdown-preview")
var markdownContent = document.getElementById("markdown-content")

var databaseRequestGoing = false
var currentDocumentId = document.body.dataset.documentId

// https://docs.djangoproject.com/en/5.1/howto/csrf/#acquiring-the-token-if-csrf-use-sessions-or-csrf-cookie-httponly-is-true
const csrftoken = document.querySelector('[name=csrfmiddlewaretoken]').value;

// TODO: Optimize the size when sending 
function updateDatabase() {

    fetch("/writing/update/" + currentDocumentId + "/",{
        method: "POST",
        headers:{
            'Content-Type': 'application/json',
            'X-CSRFToken': csrftoken
        },
        credentials: "same-origin",
        //TODO: Writing a custom md to html parser because none of the available permits to convert from html with extension to markedown with custom syntax
        body: JSON.stringify({
            md: markdownContent.innerText,
            html: markdownPreview.innerHTML,
        })
    }).finally(() => {
        databaseRequestGoing = false
    })
}

function requestDataBaseUpdate() {
    if (!databaseRequestGoing)
    {
        databaseRequestGoing = true
        setTimeout(updateDatabase,10000)
    }
}

function parseDocument() {
    markdownPreview.innerHTML = marked.parse(markdownContent.innerText)
}

const exgExtension = {
    name: 'exg',
    level: 'block',
    start(src) {
        return src.match(/^!/)?.index
    },
    tokenizer(src,tokens) {
        const match = /^! .*(?:\n|$)/.exec(src)

        if (match) {
            const token = {
                type: 'exg',
                raw: match[0],
                title: match[0].substring(2),
                tokens: []
            }

            return token
        }
    },
    renderer(token){
        return `<p class=exg-title>${token.title}</p>`;
    }
}

marked.use({extensions: [exgExtension]})

if (markdownContent != null)
{
    //Parse the document at first load
    parseDocument()
    markdownContent.addEventListener("keyup", function(_){
        requestDataBaseUpdate()
        if (markdownPreview != null)
        {
            parseDocument()
        }
    })
}
