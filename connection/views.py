from django.shortcuts import render

from django.contrib.auth import authenticate, login, logout

from django.http import HttpResponse

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
        print(f"USER FOUND : {user}")
        login(request,user)
        return HttpResponse(f"Welcome to Bexig {user.first_name} {user.last_name} @ {user.username}")
    

def try_disconnect(request):
    if request.user.is_authenticated:
        logout(request)
    return HttpResponse(request)
