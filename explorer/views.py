from django.shortcuts import render
from django.contrib.auth.models import User
from django.contrib.auth.decorators import login_required

@login_required
def explore(request):
    currentUser: User = request.user
    print(f"@{currentUser.username} ({currentUser.first_name} {currentUser.last_name}) is exploring")
    return render(request, "explorer/explore.html")