from django.http import HttpResponse
from django.template import loader
from django.shortcuts import render, get_object_or_404
from writing.models import Document

# Create your views here.
def index(request):
    return HttpResponse("Good luck writing your duck")

def edition(request, queried_document_id: int = 0):
    template = loader.get_template("writing/edition.html")
    queried_doc = get_object_or_404(Document,id=queried_document_id)
    return render(request,"writing/edition.html",{ "queried_document": queried_doc })
