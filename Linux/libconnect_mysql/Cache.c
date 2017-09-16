/*
 * Cache.c
 *
 *  Created on: 2017-9-16
 *      Author: root
 */


#include <stdio.h>
#include <stdbool.h>


#define PAGES 12  /*页面引用页数*/
#define M 3      /*当前分配给改作业的物理块数*/
//#define M 4
/*页面引用串*/
int page[PAGES] =  {4,3,2,1,4,3,5,4,3,2,1,5};
int rel[M][PAGES];   /*存储结果数组*/
/*内存物理块结构体*/
typedef struct{
  int pnum;     /*该块中所存的页面号*/
  int tm;       /*从最近一次调入所经历的时间*/
}PBlock;
/*初始化物理块数组*/
void init(PBlock *pb)
{
  int i,j;
  //pb = (PBlock*)malloc(sizeof(PBlock)*M);
  for(i=0;i<M;i++){
    pb[i].pnum = -1;
    pb[i].tm = -1;
    for(j=0;j<PAGES;j++){
      rel[i][j] = -1;
    }
  }
}
/*打印结果数组*/
void printRelArr(int rel[M][PAGES])
{
  int i,j;
  for(i=0;i<M;i++){
    for(j=0;j<PAGES;j++){
      if(rel[i][j]==-1)
        printf("_ ");
      else
        printf("%d ",rel[i][j]);
    }
    printf("\n");
  }
}
/*打印一维数组*/
void printArr1(int *arr,int n)
{
    int i;
    for(i=0;i<n;i++){
      printf("%d ",arr[i]);
    }
    printf("\n");
}
/*查看页面号为num的页面是否在内存块中,存在返回1*/
int in_mem(int num,PBlock *pb,int m)
{
  int i;
  int b = 0;
  for(i=0;i<m;i++){
      if(pb[i].pnum == num){
        b = 1;
        break;
      }
  }
  return b;
}
/*FIFO 算法的实现,无需考虑时间*/
int fifo(PBlock *pb,int m)
{
  int lps=0;   /*缺页次数*/
  double lpp;   /*缺页率*/
  int p = 0;    /*替换指针*/
  int index =0;  /*页面号索引*/
  while(index<PAGES){
      if(!in_mem(page[index],pb,M)){    //如果该页面不在物理块中
        pb[p].pnum = page[index];        /*将该页面放入物理块中*/
        p = (p+1)%M;                     /*替换指针移动*/
        lps++;                           /*却也次数加 1*/
        for(int i=0;i<M;i++){
          rel[i][index] = pb[i].pnum;
        }
      }
      index++;
  }
  printf("FIFO算法所得缺页次数为 %d\n",lps);
  lpp = (double)lps/PAGES;
  printf("FIFO算法缺页率为 %0.4lf \n",lpp);
  printf("页面号序列为:\n");
  printArr1(page,PAGES);
  printf("结果数列为:\n");
  printRelArr(rel);
  return 0;
}
/*获得最近最久的块*/
int getP(PBlock *pb,int p)
{
  int i;
  bool out = true;  //
  for(i=0;i<M;i++){
    if(pb[i].tm == -1){
      p = i;
      out = false;
      break;
    }
  }
  if(out){
    for(i=0;i<M;i++){
      if(pb[i].tm>pb[p].tm)
        p = i;
    }
  }
  return p;
}
int getEQnum(int num,PBlock *pb)
{
  int i;
  int in = -1;
  for(i=0;i<M;i++){
    if(pb[i].pnum == num){
      in = i;
      break;
    }
  }
  return in;
}
/*LRU算法*/
void lru(PBlock *pb,int m)
{
  int lps=0;   /*缺页次数*/
  double lpp;   /*缺页率*/
  int p = 0;    /*替换指针*/
  int index =0;  /*页面号索引*/
  while(index<PAGES){
      if(!in_mem(page[index],pb,m)){   /*如果页面不在物理块中*/
        p = getP(pb,p);
        pb[p].pnum = page[index];
        pb[p].tm = 0;
        lps++;
        for(int i=0;i<M;i++){
          rel[i][index] = pb[i].pnum;
        }
      }else{                         /*如果页面在物理块中*/
        int in = getEQnum(page[index],pb);  /*获取该页面在物理块中的索引*/
          pb[in].tm = 0;
      }
      int i;
      for(i=0;i<M;i++){
          if(i!=p&&pb[i].tm!=-1){
            pb[i].tm++;
          }
      }
      index++;
  }
  printf("LRU算法所得缺页次数为 %d \n",lps);
  lpp = 1.0*lps/PAGES;
  printf("LRU算法缺页率为: %0.4lf\n",lpp);
  printf("页面号序列为:\n");
  printArr1(page,PAGES);
  printf("LRU结果数组为:\n");
  printRelArr(rel);
}
//int main()
//{
//    //printArr(rel);
//  PBlock pb[M];
//  init(pb);
//  fifo(pb,M);
//  init(pb);
//  lru(pb,M);
//  return 0;
//}
