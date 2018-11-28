#include <stdio.h>
#include <stdlib.h>

typedef unsigned int UINT;
#define MEM_SIZE (1 << 20)
UINT memPool[MEM_SIZE];

//双向链表
struct VListHead {
  VListHead() { initListHead(); }
  void addListNext(VListHead *head) { addFreeList(head, head->next); }
  //将当前节点从链表中删除
  void delList() {
    removeFromList(prev, next);
    next = NULL;
    prev = NULL;
  }
  void initListHead() {
    next = this;
    prev = this;
  }
  void addFreeList(VListHead *prv, VListHead *nxt) {
    nxt->prev = this;
    this->next = nxt;
    this->prev = prv;
    prv->next = this;
  }
  //移除prv和nxt之间的节点
  static void removeFromList(VListHead *prv, VListHead *nxt) {
    nxt->prev = prv;
    prv->next = nxt;
  }
  VListHead *next;
  VListHead *prev;
};

VListHead listHead;
#define SetBlockHeadSize(pInfo, dwords) ((*(pInfo) &= 0x3) |= ((dwords) << 2))
#define GetBlockHeadSize(pInfo) (*(pInfo) >> 2)  //返回DWORD数
#define BLOCK_USE_MASK (0x1)  //最低位标记块是否使用
#define IsBlockUsed(pInfo) (*(pInfo)&BLOCK_USE_MASK)
#define UseBlock(pInfo) (*(pInfo) |= BLOCK_USE_MASK)
#define UnUseBlock(pInfo) (*(pInfo) &= ~BLOCK_USE_MASK)

void setBlockFoot(UINT *pBlockHead) {  //设置内存块尾部信息字
  pBlockHead[(*pBlockHead >> 2) - 1] = pBlockHead[0];
}

//用于遍历隐式链表
UINT *nextBlock(UINT *pBlockHead) {  //到下一个内存块
  return &pBlockHead[*pBlockHead >> 2];
}
UINT *prevBlock(UINT *pBlockHead) {  //到上一个内存块
  return &pBlockHead[-(*(pBlockHead - 1) >> 2)];
}

//空闲表节点的加入和删除
void addFreeList(UINT *pBlockHead) {
  ((VListHead *)(pBlockHead + 1))->addListNext(&listHead);
}
void delFreeList(UINT *pBlockHead) {
  ((VListHead *)(pBlockHead + 1))->delList();
}

//得到当前分配的内存大小(包括内部碎片)
UINT getAllocDwords() {
  UINT *ptr = nextBlock(memPool);
  UINT dwords = 0;
  //遍历隐式表
  int blkSz;
  while (blkSz = GetBlockHeadSize(ptr)) {
    if (IsBlockUsed(ptr)) {
      dwords += blkSz;
    }
    ptr = nextBlock(ptr);
  }
  return dwords;
}

//初始化空闲表和隐式链表
void initMemPool() {
  //先建立隐式表头
  UINT *ptr = memPool;
  UseBlock(ptr);
  SetBlockHeadSize(ptr, 2);  //占用2个DWORD
  setBlockFoot(ptr);

  //设置表体
  ptr = nextBlock(ptr);
  UnUseBlock(ptr);
  SetBlockHeadSize(ptr, MEM_SIZE - 3);
  setBlockFoot(ptr);

  //加入空闲表
  addFreeList(ptr);

  //设置表尾
  ptr = nextBlock(ptr);
  UseBlock(ptr);
  SetBlockHeadSize(ptr, 0);
}

void *Malloc(UINT sz) {
  void *pRet = NULL;
  VListHead *pTmpList = listHead.next;
  sz = (sz + 3) / 4 + 2;  //转换为DWORD数，再加上2个DWORD的附加信息
  if (sz < 4) sz = 4;     //最小块是4个DWORD
  while (pTmpList != &listHead) {  //遍历空闲表(首次匹配)
    UINT *pBlockHead = (UINT *)pTmpList - 1;
    UINT objSz = GetBlockHeadSize(pBlockHead);
    if (objSz >= sz) {  //找到满足要求的空闲块
      pRet = pTmpList;
      UseBlock(pBlockHead);  //设置使用标记
      pTmpList->delList();   //从空闲表中去除
      //判断是否要分割
      if (objSz >=
          sz + 4) {  //最小分割块的大小是4个DWORD（用于建立隐式表和空闲表）
        SetBlockHeadSize(pBlockHead, sz);
        setBlockFoot(pBlockHead);
        pBlockHead = nextBlock(pBlockHead);  //将新的空闲块加入空闲表
        UnUseBlock(pBlockHead);
        SetBlockHeadSize(pBlockHead, objSz - sz);
        setBlockFoot(pBlockHead);
        addFreeList(pBlockHead);  //加入空闲表
      } else {
        SetBlockHeadSize(pBlockHead, objSz);
        setBlockFoot(pBlockHead);
      }
      break;
    }
    pTmpList = pTmpList->next;
  }
  return pRet;
}

void Free(void *pObj) {
  if (pObj) {
    UINT *pBlockHead = (UINT *)pObj - 1;
    UINT *pPrevBlock = prevBlock(pBlockHead);
    UINT *pNextBlock = nextBlock(pBlockHead);
    UnUseBlock(pBlockHead);  //更新标记
    //判断是否需要合并
    if (!IsBlockUsed(pNextBlock)) {
      delFreeList(pNextBlock);
      int newSz = GetBlockHeadSize(pBlockHead) + GetBlockHeadSize(pNextBlock);
      SetBlockHeadSize(pBlockHead, newSz);
    }
    if (IsBlockUsed(pPrevBlock)) {
      //将当前空闲块链入空闲表
      addFreeList(pBlockHead);
      setBlockFoot(pBlockHead);
    } else {
      int newSz = GetBlockHeadSize(pBlockHead) + GetBlockHeadSize(pPrevBlock);
      SetBlockHeadSize(pPrevBlock, newSz);  //直接合并到前一个内存块
      setBlockFoot(pPrevBlock);
    }
  }
}

/////////////////////////////////////测试部分////////////////////////////////
#define TEST_MAX (10000)
char *test[TEST_MAX];
int main(int argc, char *argv[]) {
  initMemPool();
  srand((int)srand);
  for (int i = 0; i < TEST_MAX; ++i) {
    if (!(test[i] = (char *)Malloc(rand() % MEM_SIZE))) {
      //计算内存使用率
      printf("using  ratio:%f\n", getAllocDwords() / (float)MEM_SIZE);
      int freeIdx = rand() % i;
      while (!test[freeIdx]) {
        freeIdx = (freeIdx + 1) % i;
      }
      Free(test[freeIdx]);
      test[freeIdx] = NULL;
    }
  }
  return 0;
}