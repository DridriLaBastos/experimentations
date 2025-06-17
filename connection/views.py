from django.shortcuts import render, redirect

from django.contrib.auth import authenticate, login, logout

from django.http import HttpResponse

from django.urls import reverse

# Create your views here.
def connect(request):
    return render(request, "connection/connect.html")

def try_connect(request):
    username = request.POST["username"]
    password = request.POST["password"]
    
    print(f"User trying to connect : {username} with {password}")
    
    user = authenticate(request, username=username, password=password)
    
    if user is None:
        print("USER NOT FOUND")
        return HttpResponse("User not found")
    else:
        login(request,user)
        return redirect("explorer:explore")

def try_disconnect(request):
    if request.user.is_authenticated:
        logout(request)
    return redirect("connection:home")
