from django.http import HttpResponse
from django.shortcuts import render
from django.contrib.auth.decorators import login_not_required

from django.contrib.auth import authenticate, login


# TODO: How to use Django asgi instead of wsgi ?
# investigations can start here https://docs.djangoproject.com/en/6.1/howto/deployment/asgi/
# and here for login example : https://docs.djangoproject.com/en/6.1/topics/auth/default/#authentication-in-web-requests
# @login_not_required
# def login(request):
#     username = request.POST["username"]
#     password = request.POST["password"]

#     user = authenticate(request, username=username, password=password)

#     if user is not None:
#         print(f"Found {user}")
#         login(request, user)
#     else:
#         print(f"User {username} not found")

# def logout(request):
#     logout(request)
