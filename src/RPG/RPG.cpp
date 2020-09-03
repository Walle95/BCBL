/**
* =============================================================================
* Rotary Pulse Generator class (Version 0.0.1)
* =============================================================================
* Copyright (c) 2012 Christopher Anderson
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* =============================================================================
*/
 
#include "RPG.h"
#include "mbed.h"

/**
 * Constructor(Channel A input pin, Channel B input pin, Pushbutton input pin)
 */
RPG::RPG(PinName pA, PinName pB, PinName pPB)
    : chRPG(pA, pB), PB(pPB)
    {
        chRPG.input();
        chRPG.mode(PullUp);
        PB.mode(PullUp);
        wait_us(10000);
        RPGO = chRPG;
    }
    
/**
 *Destructor
 */
RPG::~RPG()
{}

/**
 *reads and debounces push button returns bool result
 */
bool RPG::pb()
{
    int check = PB;
    wait_us(5000);
    if((!check) && !PB)
    {
        return true;
    }
    else return false;
}

/**
 *Determines direction of rotation returns:
 *1 for clockwise
 *-1 for counter-clockwise
 *0 for no rotation
 */
int RPG::dir()
{
    int dir = 0;
    RPGO = chRPG;
    wait_us(1000);
    RPGN = chRPG;
    if(RPGN != RPGO)
    {
        if((RPGN & 1) != (RPGO >> 1)) dir = 1;
        else dir = -1;
    }
    else dir = 0;
    RPGO = RPGN;
    return dir;
}