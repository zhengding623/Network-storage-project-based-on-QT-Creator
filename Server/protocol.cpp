#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


PDU *mkPDU(uint uiMsgLen)//创建pdu函数
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen;//消息总长度为参数长度 + 消息长度
    PDU* pdu = (PDU*)malloc(uiPDULen);//申请空间
    if(pdu == NULL){//判空，为空退出
        exit(1);
    }
    memset(pdu, 0, uiPDULen);//置零
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;

    return pdu;
}
