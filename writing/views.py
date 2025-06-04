from django.shortcuts import render, get_object_or_404
from django.http import HttpResponse

from .models import Document

import json

def edition(request, requested_document_id: int = 0):
    requestedDocument: Document = get_object_or_404(Document, id=requested_document_id)
    return render(request,"writing/edition.html", { "requested_document": requestedDocument })

# TODO: Exception handling
def update(request, document_id: int = 0):
    data = json.loads(request.body)
    #print(f"{data.get("md")}\n{data.get("html")}")
    print(f"{request}")
    Document.objects.filter(id=document_id).update(mddata=data.get("md"), htmldata=data.get("html"))
    return HttpResponse(request)
