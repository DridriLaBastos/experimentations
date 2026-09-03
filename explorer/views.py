from django.shortcuts import render
from django.contrib.auth.models import User
from django.contrib.auth.decorators import login_required

from writing.models import DocumentMembership
from writing.models import Document

@login_required
def explore(request):
    loggedUser: User = request.user
    #TODO Is there a one liner to do this ?
    authorizedDocumentIds = DocumentMembership.objects.filter(user_id=loggedUser).values("document_id")  
    authorizedDocumentList = list(Document.objects.filter(id__in=authorizedDocumentIds).values("id", "name"))
    print(authorizedDocumentList)
    
    context = {
        "authorized_document_list": authorizedDocumentList
    }
    
    return render(request, "explorer/explore.html",context)