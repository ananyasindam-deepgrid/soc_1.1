//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "soc.h"
#include "../base/dgrid.h"
#include "../base/util.h"
#include "../apps/llm/llm.h"


#ifndef __WIN32__
static char *getInput()
{
    static int inputLen=0;
    static char input[256];
    char ch;

    printf("\r\n>");
    fflush(stdout);
    while(UartReadAvailable())
        ch = UartRead();
    inputLen = 0;
    for(;;) {    
        if(UartReadAvailable()) {
            ch = UartRead();
            printf("%c",ch);
            fflush(stdout);
            if(ch==0x3) {
                input[0] = 0x3;
                input[1] = 0;
                return input;
            }
            else if(ch=='\n' || ch=='\r') {
                printf("\r\n");
                fflush(stdout);
                input[inputLen]=0;
                return input;
            } else if(ch=='\b') {
                if(inputLen > 0)
                    inputLen--;
            } else {
                if(inputLen < (sizeof(input)-1)) {
                    input[inputLen++]=ch;
                }
            } 
        }
    }
    return 0;
}
#endif

static llama ai;

#define CHAT_MODEL_LLAMA   "LLAMA.ZUF"
#define CHAT_MODEL_SMOLLM2 "SMOLLM2.ZUF"

int chat() {
    static std::string output_ref_0,output_ref_1;
    static std::string output;
    int failCount=0;
    int goodCount=0;
    int i;

#if 0
      ai.Open(CHAT_MODEL_LLAMA);
#endif

#if 1
      ai.Open(CHAT_MODEL_SMOLLM2);
#endif

      ai.SetSamplingPolicy(0.5,0.8); // temperature=0.8,threshold=0.9
//      ai.SetSamplingPolicyGreedy();
#ifdef __WIN32__
    ai.SystemPrompt((char*)"You are a helpful assistant.");
    for(i=0;;i++) {
        ai.Clear();

        output.clear();
//        ai.UserPrompt((char*)"What is the capital of Egypt",&output);
        ai.UserPrompt((char*)"Who is Issac Newton", &output);

        if(i==0)
            output_ref_0 = output;
        if (output_ref_0 != output) {
            failCount++;
        }
        else {
            goodCount++;
        }
#if 0
        output.clear();
        ai.UserPrompt((char*)"And of USA?", &output);
        if (i == 0)
            output_ref_1 = output;
        if (output_ref_1 != output) {
            failCount++;
        }
        else {
            goodCount++;
        }
#endif
        printf("\r\n--> SUCESS fail=%d good=%d \r\n",failCount,goodCount);
    }
    ai.Close();
#endif
#ifndef __WIN32__
    printf("I am a chatbot. Hit Ctrl+C to interrupt me.\r\n");
    ai.SystemPrompt((char*)"You are a helpful assistance");
    for(;;) {
        char *prompt = getInput();
        if(prompt) {
            if(prompt[0]==0x3)
                ai.Clear();
            else {
                // Since this is a small model. It does not handle long context well.
                // Clear previous context before answering new query
                ai.Clear(); 
                ai.ClearStat();
                ai.UserPrompt(prompt,0);
                printf(" (tok/sec=%.2f)",ai.GetStatTokPerSec());
            }
        }
    }
#endif
    return 0;
}
