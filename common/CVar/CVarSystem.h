/*
# MIT License

# Copyright(c) 2018-2019 NovusCore

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/
#pragma once
#include <NovusTypes.h>

class CVarParameter;

class CVarSystem {

public:
    static CVarSystem* Get();
   

    //pimpl

    virtual CVarParameter* GetCVar(const char* name) = 0;
    virtual CVarParameter* GetCVar(u32 namehash) = 0;

    
    virtual float* GetFloatCVar(const char* name) = 0;
    virtual float* GetFloatCVar(u32 namehash) = 0;


    virtual int* GetIntCVar(const char* name) = 0;
    virtual int* GetIntCVar(u32 namehash) = 0;

    virtual const char* GetStringCVar(const char* name) = 0;
    virtual const char* GetStringCVar(u32 namehash) = 0;

    virtual void SetFloatCVar(const char* name, float value) = 0;
    virtual void SetFloatCVar(u32 namehash, float value) = 0;
   
    virtual void SetIntCVar(const char* name, int value) = 0;
    virtual void SetIntCVar(u32 namehash, int value) = 0;
   
    virtual void SetStringCVar(const char* name, const char* value) = 0;
    virtual void SetStringCVar(u32 namehash, const char* value) = 0;

    virtual CVarParameter* CreateFloatCVar(const char* name, const char* description,float defaultValue) = 0;
    virtual CVarParameter* CreateIntCVar(const char* name, const char* description, int defaultValue) = 0;
    virtual CVarParameter* CreateStringCVar(const char* name, const char* description, const char * defaultValue) = 0;
};

struct AutoCVar_Float {
    AutoCVar_Float(const char* name, const char* description, float defaultValue);

    float Get();
    void Set(float val);

    CVarParameter* cvar;
};

struct AutoCVar_Int {
    AutoCVar_Int(const char* name, const char* description, int defaultValue);
    int Get();
    void Set(int val);
    
    void Toggle();

    CVarParameter* cvar;
};

struct AutoCVar_String {
    AutoCVar_String(const char* name, const char* description, const char * defaultValue);

    CVarParameter* cvar;
};