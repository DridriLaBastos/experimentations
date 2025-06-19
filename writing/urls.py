from django.urls import path

from . import views

app_name = "writing"

# Use a path like /docs/<docid> to access a document
# https://docs.djangoproject.com/en/4.2/intro/tutorial03/#writing-more-views
urlpatterns = [
    path("",views.edition,name="default_document_edition"),
    path("<int:requested_document_id>/",views.edition,name="document_edition"),
    path ("update/<int:document_id>/",views.update,name="document_update")
]
