from django.db import models

from django.contrib.auth.models import User

# Create your models here.
class Document(models.Model):
    name     = models.CharField("document_name",max_length=256)
    creation = models.DateTimeField("creation_date")
    modif    = models.DateTimeField("last_modified")
    mddata   = models.TextField("markedown_content", default="")
    htmldata = models.TextField("html_content", default="")
    members  = models.ManyToManyField(User, through="DocumentMembership")
    
    class Meta:
        db_table='documents'
    
    def __str__(self):
        return f"'{self.name}'"

class DocumentMembership(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    document = models.ForeignKey("Document", on_delete=models.CASCADE)
    write = models.BooleanField(verbose_name="has_write_permission", name="write", default=False)
    
    class Meta:
        db_table = "document_membership"
