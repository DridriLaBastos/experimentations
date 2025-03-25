from django.http import HttpResponse
from django.template import loader
from django.shortcuts import render

# Create your views here.
def index(request):
    return HttpResponse("Good luck writing your duck")

def edition(request):
    template = loader.get_template("writing/edition.html")
    return render(request,"writing/edition.html")
