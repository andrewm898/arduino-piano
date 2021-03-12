#include <Tone.h>

Tone tone1;
Tone tone2;

const int button1Pin = 7;
const int button2Pin = A1;
const int button3Pin = A2;
const int button4Pin = A3;
const int button5Pin = A4;
const int button6Pin = A5;
const int button7Pin = 6;
const int button8Pin = 5;
const int button9Pin = 4;
const int button10Pin = 3;
const int button11Pin = 2;
const int button12Pin = 1;
const int button13Pin = 0;

const uint16_t
    grNotes[] =
    {
        NOTE_C4,       //corresponds to button 1
        NOTE_CS4,      // this is button 2
        NOTE_D4,       // this is button 3
        NOTE_DS4,      // this is button 4
        NOTE_E4,       // this is button 5
        NOTE_F4,       // this is button 6
        NOTE_FS4,      // this is button 7
        NOTE_G4,       // this is button 8
        NOTE_GS4,      // this is button 9
        NOTE_A4,       // this is button 10
        NOTE_AS4,      // this is button 11
        NOTE_B4,        // this is button 12
        NOTE_C5        // this is button 13
    };
   
uint8_t
    spkrMask;

typedef struct structButtonInfo
{
    uint8_t     pin;            //pin button
    uint8_t     usedMask;       //mask used to re-set speaker bit
    Tone        *pTone;         //pointer to the tone output that will play this button's note
    uint8_t     lastRead;       //last read value
    uint32_t    timeRead;       //used for timing uS between reads
   
}sButtonInfo_t;

sButtonInfo_t
    ButtonInfo[13] =
    {
        {
            //button 1
            .pin = button1Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 2
            .pin = button2Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 3
            .pin = button3Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 4
            .pin = button4Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 5
            .pin = button5Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 6
            .pin = button6Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 7
            .pin = button7Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 8
            .pin = button8Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 9
            .pin = button9Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 10
            .pin = button10Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 11
            .pin = button11Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 12
            .pin = button12Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        },
        {
            //button 13
            .pin = button13Pin,
            .usedMask = 0,
            .pTone = NULL,
            .lastRead = LOW,
            .timeRead = 0ul
        }
    };
   

void setup( void )
{
    Serial.begin(9600);
   
    // tone1 plays sound on the speaker connected to Pin 13,
    // tone 2 for the speaker on pin 11.
    tone1.begin(13);
    tone2.begin(12);
   
    // Using INPUT_PULLUP to make configuring the buttons work- this makes their default state
    // HIGH, so to "press" them they are connected to ground, and switch to LOW
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
    pinMode(button4Pin, INPUT_PULLUP);
    pinMode(button5Pin, INPUT_PULLUP);
    pinMode(button6Pin, INPUT_PULLUP);
    pinMode(button7Pin, INPUT_PULLUP);
    pinMode(button8Pin, INPUT_PULLUP);
    pinMode(button9Pin, INPUT_PULLUP);
    pinMode(button10Pin, INPUT_PULLUP);
    pinMode(button11Pin, INPUT_PULLUP);
    pinMode(button12Pin, INPUT_PULLUP);
    pinMode(button13Pin, INPUT_PULLUP);

    //in spkrMask, bits 0 and 1 are used to indicate speaker status
    //if bit 0 == 1, spkr 1 is free; if bit 0 == 0, spkr 1 is in use
    //if bit 1 == 1, spkr 2 is free; if bit 1 == 0, spkr 2 is in use
    //init to 0x03 to indicate both speakers free
    spkrMask = 0x03;

    //establish the "last" value of the buttons with this initial read
    for( uint8_t i=0; i<13; i++ )
        ButtonInfo[i].lastRead = digitalRead( ButtonInfo[i].pin );
   
}//setup

void loop( void )
{
    PlayNotes();
           
}//loop

void PlayNotes( void )
{
    static uint8_t
        index = 0;   
    uint32_t
        timeNow = micros();

    //read switches every 2mS (2000uS)
    if( (timeNow - ButtonInfo[index].timeRead) >= 2000ul )
    {
        //time for a read; save the micros count now for next read
        ButtonInfo[index].timeRead = timeNow;
        //read the switch
        uint8_t nowRead = digitalRead( ButtonInfo[index].pin );
        //if not the same as last time...
        if( nowRead != ButtonInfo[index].lastRead )
        {
            //save new value as last
            ButtonInfo[index].lastRead = nowRead;
            //if low, button has been pressed
            if( nowRead == LOW )
            {
                //see switch speaker is available
                switch( spkrMask )
                {
                    //if speaker mask is 0b11 or 0x01, then both or only #1 is available
                    //in either case, use speaker 1
                    case    0x03:
                    case    0x01:
                        //show that speaker 1 is now occupied by clearing bit 0 in the mask
                        spkrMask &= 0b00000010;
                        //and assign tone1 to this button
                        ButtonInfo[index].pTone = &tone1;
                        //this is the mask we use to reinstate the bit when the note is finished
                        //here we'll re-set bit 0
                        ButtonInfo[index].usedMask = 0b00000001;
                       
                    break;

                    case    0x02:
                        //spkr 1 in use; select spkr 2
                        //same logic as case 3/1 but now spkr 2 is the one we use
                        spkrMask &= 0b00000001; //clear bit 1
                        ButtonInfo[index].pTone = &tone2;   //assign tone2
                        ButtonInfo[index].usedMask = 0b00000010;    //re-set mask is for bit 1
                       
                    break;

                    default:
                        //invalid or no spkr available
                        //no tone is given and no bits are set when button released
                        ButtonInfo[index].pTone = NULL;
                        ButtonInfo[index].usedMask = 0b00000000;
                       
                    break;
                   
                }//switch

                //if a tone pin was assigned...
                if( ButtonInfo[index].pTone )
                    //play the note for this button at the tone output assigned above
                    ButtonInfo[index].pTone->play( grNotes[index] );
               
            }//if
            else
            {
                //here is key was just released
                //if we're playing a note...
                if( ButtonInfo[index].pTone )
                    //stop it now
                    ButtonInfo[index].pTone->stop();               

                //re-set the speaker mask bit (if used; if none was available, 0x00 is ORd in and nothing changes)
                spkrMask |= ButtonInfo[index].usedMask;
                //clear the re-set mask for this button
                ButtonInfo[index].usedMask = 0;
               
            }//else
           
        }//if
       
    }//if

    //each pass we look at one button.
    //bump button index each pass
    index++;
    if( index == 13 )
        index = 0;
   
}//PlayNotes
