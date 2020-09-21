#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
struct graph
{
	int vertex;
	struct graph *next;
}*front=NULL,*rear=NULL;
struct result
{
	int content;
	int distance;
};
struct contentus
{
	int content;
	int user;
};
void enqueue(int item)
{
	struct graph *temp;
    if (rear == NULL)
    {
        rear = (struct graph *)malloc(sizeof(struct graph));
        rear->next = NULL;
        rear->vertex = item;
        front = rear;
    }
    else
    {
        temp=(struct graph *)malloc(sizeof(struct graph));
        rear->next = temp;
        temp->vertex = item;
        temp->next = NULL;
        rear = temp;
    }
}
int dequeue()
{
    int item;
    struct graph *temp;
    if(front==NULL)
        return -1;
    else
    {
        temp=front;
        item=front->vertex;
        front=front->next;
        if(front==NULL)
        	rear=NULL;
        free(temp);
        return item;
    }
}

void initialize(struct graph **graph1,int length)
{
	int i;
	for(i=0;i<length;i++)
		graph1[i]=NULL;
}
int create_graph(struct graph **graph1, int arr[][1000],int noele,int *length)
{
	int i,j;
	struct graph *temp;
	initialize(graph1,noele);
	for(i=0;i<noele;i++)
	{
	for(j=0;j<length[i];j++)
	{
		temp=(struct graph*)malloc(sizeof(struct graph));
		temp->next=NULL;
		temp->vertex = arr[i][j];
		if(graph1[i]==NULL)
			graph1[i]=temp;
		else
		{
			temp->next=graph1[i];
			graph1[i]=temp;
		}
	}
	}
	return 1;
}
void print(struct graph **graph1, int length)
{
	struct graph *temp;
	int i;
	for(i=0;i<length;i++)
	{
		temp=graph1[i];
		while(temp!=NULL)
		{
			printf("%d ",temp->vertex);
			temp=temp->next;
		}
		printf("\n");
	}
}
int create_content_user(struct graph **content_user,struct graph **content,int length,int totlength)
{
	int i;
	struct graph *temp,*temp1;
	initialize(content_user,length);
	for(i=0;i<totlength;i++)
	{
		temp=content[i];
		while(temp!=NULL)
		{
			temp1=(struct graph *)malloc(sizeof(struct graph));
			temp1->next=NULL;
			temp1->vertex=i;
			if(content_user[temp->vertex]==NULL)
				content_user[temp->vertex]=temp1;
			else
			{
				temp1->next=content_user[temp->vertex];
				content_user[temp->vertex]=temp1;
			}
			temp=temp->next;
		}
	}
}
int connectedUserNodes(struct graph **graph1,struct graph **content,struct graph **content_user,int nocontent,struct result *final,int knn,int kdistcontent,int *knu, int *knud,int main_user)
{
	int i,j=0,k,flag=0,start,parent,distance;
	front=NULL,rear=NULL;
	struct graph *temp;
	struct contentus onedistnodes[100];
	struct result nearest[100][100];
	int visited[100],score[100];
	for(i=0;i<kdistcontent;i++)
	{
		temp=content_user[final[i].content];
		while(temp!=NULL)
		{	
			onedistnodes[j].user=temp->vertex;
			onedistnodes[j++].content=final[i].content;
			temp=temp->next;
		}
	}
	for(i=0;i<100;i++)
	{	for(k=0;k<100;k++)
		{
			nearest[i][k].content=-1;
			nearest[i][k].distance=-1;
		}
	}
	//print(graph1,5);
	//print(content,7);
	for(i=0;i<j;i++)
	{
		for(k=0;k<j;k++)
		visited[k]=-1;
		start=onedistnodes[i].user;
		visited[start]=-2;
		nearest[i][0].content=start;
		nearest[i][0].distance=0;
		k=1;
		parent=start;
		distance=1;
		front=NULL,rear=NULL;
		while(start!=-1 && distance!=knn)
		{
		temp=graph1[start];
		while(temp!=NULL)
		{
			if(visited[temp->vertex]==-1)
			{
				(nearest[i][k]).distance=distance;
				(nearest[i][k++]).content=temp->vertex;
				enqueue(temp->vertex);
				visited[temp->vertex]=parent;
			}
			temp=temp->next;
		}
		start=dequeue();
		if(visited[start]==parent)
		{
			distance++;
			parent=start;
		}
		}
	}
	for(i=0;i<j;i++)
	{
		k=0;
		while(nearest[i][k].content!=-1)
			printf("%d ",nearest[i][k++].content);
		printf("\n");
	}
	for(k=0;k<100;k++)
		score[k]=0;
	for(i=0;i<j;i++)
	{
		k=0;
		while(nearest[i][k].content!=-1)
		{
				if(nearest[i][k].content!=main_user)
					score[nearest[i][k].content]++;
				k++;
		}
	}
	for(i=0;i<j;i++)
	{
		if(i!=3)
			printf("user %d , distance %d\n",i,score[i]);
	}
	for(i=0;i<j;i++)
		printf("%d-----%d\n",onedistnodes[i].user,onedistnodes[i].content);
	return 0;
}
int distanceLContentNodes(struct graph **content,int nocontent,struct graph **graph1,struct graph **content_user, int knn,int start, int *knu, int *knud,int totvertex)
{
	int i,*visited,distance=1,parent,marker,j,k,flag,l,main_user;
	visited=(int *)malloc(totvertex*sizeof(int));
	struct result final[100];
	struct graph *temp,*temp1;
	for(i=0;i<totvertex;i++)
		visited[i]=-1;
	for(i=0;i<knn;i++)
		knu[i]=-1;
	main_user=start;
	marker=start;
	visited[start]=-2;
	knu[0]=start;
	knud[0]=0;
	i=1;
	//printf("knn==%d\n",knn);
	parent=start;
	while(start!=-1 && distance!=knn)
	{
	temp=graph1[start];
	while(temp!=NULL)
	{
		if(visited[temp->vertex]==-1)
		{
			knud[i]=distance;
			knu[i++]=temp->vertex;
			enqueue(temp->vertex);
			visited[temp->vertex]=parent;
		}
		temp=temp->next;
	}
	start=dequeue();
	if(visited[start]==parent)
	{
		distance++;
		parent=start;
	}
	}
	k=0,j=0;
	//printf("/-/-/-//-/-/-/-/\n");
	//for(k=0;k<totvertex;k++)
	//	printf("%d ",visited[k]);
	//printf("\n!@#!@!#!@!!@\n");
	k=0;
	while(j!=totvertex && knud[j]!=knn)
	{
		temp=content[knu[j]];
		while(temp!=NULL && distance<=knn)
		{
			for(l=0;l<k;l++)
			{
				if(temp->vertex==final[l].content)
				{
					marker=1;
					break;
				}
			}
			if(marker==1)
			{
				marker=0;
				temp=temp->next;
				continue;
			}
			final[k].content=temp->vertex;
			final[k].distance=knud[j]+1;
			k++;
			temp=temp->next;
		}
		j++;	
	}
	for(j=0;j<k;j++)
		printf("content %d , distance %d\n",final[j].content,final[j].distance);
	connectedUserNodes(graph1,content, content_user, nocontent,final,knn,k,knu,knud,main_user);
	return i;

}
int main(int argc, char *argv[])
{
    FILE *fp;
    char str[100], *token;
    int nouser,nocontent,userl[100],main_user,user[100][1000],contentl[100],content[100][1000],i=0,j=0,index,knu[100],knud[100],knn;
    struct graph *user_graph[100];
    struct graph *content_graph[100];
	struct graph *content_user[100];
    fp = fopen("input2.txt","r");
    main_user=atoi("3");
    knn=atoi("3");
    if (fp == NULL)
    {
        printf("ERROR READING FILE\n");
        return 0;
    }
    fscanf(fp,"%d",&nouser);
    for(i=0;i<nouser;i++)
    {
    	j=0;
    	fscanf(fp, "%s", str);
    	token =strtok(str, ":");
    	token =strtok(NULL, ",");
    	while(token!=NULL)
    	{
    		user[i][j] = atoi(token);
    		token = strtok(NULL,",");
    		j++;
    	}
    	userl[i]=j;
    }
    fscanf(fp,"%d",&nocontent);
    for(i=0;i<nouser;i++)
    {
    	j=0;
    	fscanf(fp, "%s", str);
    	token =strtok(str, ":");
    	token =strtok(NULL, ",");
    	while(token!=NULL)
    	{
    		content[i][j] = atoi(token);
    		token = strtok(NULL,",");
    		j++;
    	}
    	contentl[i]=j;
    }
    fclose(fp);
    create_graph(user_graph,user,nouser,userl);
    //print(user_graph,nouser);
    //printf("\n");
    create_graph(content_graph,content,nouser,contentl);
	create_content_user(content_user,content_graph,nocontent,nouser);
	//printf("/-/-/-/-/-/\n");
	//print(content_user,nocontent);
	//printf("/-/-/-/-/-/\n");
    //print(content_graph,nouser);
    knn=distanceLContentNodes(content_graph,nocontent,user_graph,content_user,knn,main_user, knu, knud,nouser);
    //printf("\n");
	printf("\n/*/*/*//*/\n");
    for(i=0;i<knn;i++)
    	printf("%d ",knu[i]);
    printf("\n");
    for(i=0;i<knn;i++)
    	printf("%d ",knud[i]);
	printf("\n/*/*/*//*/\n");
    return 0;
}
