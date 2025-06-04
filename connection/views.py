from django.shortcuts import render

from django.http import HttpResponse

# Create your views here.
def connect(request):
    return render(request, "connection/connect.html")
