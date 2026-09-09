from django.shortcuts import render, get_object_or_404, redirect
from django.http import HttpResponse, HttpResponseNotAllowed
from django.contrib.auth.decorators import login_required
from django.utils import timezone

from .models import Document, DocumentMembership

import json

@login_required
def edition(request, requested_document_id: int = 0):
    requestedDocument: Document = get_object_or_404(Document, id=requested_document_id)
    return render(request,"writing/edition.html", { "requested_document": requestedDocument })

# TODO: Exception handling
@login_required
def update(request, document_id: int = 0):
    data = json.loads(request.body)
    #print(f"{data.get("md")}\n{data.get("html")}")
    print(f"{request}")
    Document.objects.filter(id=document_id).update(mddata=data.get("md"), htmldata=data.get("html"))
    return HttpResponse(request)

@login_required
def new_document(request):
    if request.method != "POST":
        return HttpResponseNotAllowed(["POST"])

    now = timezone.now()
    created = Document.objects.create(
        name="Untitled document",
        creation=now,
        modif=now,
    )
    DocumentMembership.objects.create(
        user=request.user,
        document=created,
        write=True,
    )
    return redirect("writing:document_edition", requested_document_id=created.id)