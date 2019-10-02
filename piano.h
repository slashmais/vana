#ifndef _vana_piano_h_
#define _vana_piano_h_


#include <VIO/vio.h>


//piano-key frequencies
extern double pk_88; // = 4186.01; //last key on right-hand side //can't reach in voice mode
extern double pk_87; // = 3951.07;
extern double pk_86; // = 3729.31;
extern double pk_85; // = 3520.0;
extern double pk_84; // = 3322.44;
extern double pk_83; // = 3135.96;
extern double pk_82; // = 2959.96;
extern double pk_81; // = 2793.83;
extern double pk_80; // = 2637.02;
extern double pk_79; // = 2489.02;
extern double pk_78; // = 2349.32;
extern double pk_77; // = 2217.46;
extern double pk_76; // = 2093.0;
extern double pk_75; // = 1975.53;
extern double pk_74; // = 1864.66;
extern double pk_73; // = 1760.0;
extern double pk_72; // = 1661.22;
extern double pk_71; // = 1567.98;
extern double pk_70; // = 1479.98;
extern double pk_69; // = 1396.91;
extern double pk_68; // = 1318.51;
extern double pk_67; // = 1244.51;
extern double pk_66; // = 1174.66;
extern double pk_65; // = 1108.73;
extern double pk_64; // = 1046.5;
extern double pk_63; // = 987.767;
extern double pk_62; // = 932.328;
extern double pk_61; // = 880.0;
extern double pk_60; // = 830.609;
extern double pk_59; // = 783.991;
extern double pk_58; // = 739.989;
extern double pk_57; // = 698.456;
extern double pk_56; // = 659.255;
extern double pk_55; // = 622.254;
extern double pk_54; // = 587.33;
extern double pk_53; // = 554.365;
extern double pk_52; // = 523.251;
extern double pk_51; // = 493.883;
extern double pk_50; // = 466.164;
extern double pk_49; // = 440.0;
extern double pk_48; // = 415.305;
extern double pk_47; // = 391.995;
extern double pk_46; // = 369.994;
extern double pk_45; // = 349.228;
extern double pk_44; // = 329.628;
extern double pk_43; // = 311.127;
extern double pk_42; // = 293.665;
extern double pk_41; // = 277.183;
extern double pk_40; // = 261.626; //middle C
extern double pk_39; // = 246.942;
extern double pk_38; // = 233.082;
extern double pk_37; // = 220.0;
extern double pk_36; // = 207.652;
extern double pk_35; // = 195.998;
extern double pk_34; // = 184.997;
extern double pk_33; // = 174.614;
extern double pk_32; // = 164.814;
extern double pk_31; // = 155.563;
extern double pk_30; // = 146.832;
extern double pk_29; // = 138.591;
extern double pk_28; // = 130.813;
extern double pk_27; // = 123.471;
extern double pk_26; // = 116.541;
extern double pk_25; // = 110.0;
extern double pk_24; // = 103.826;
extern double pk_23; // = 97.9989;
extern double pk_22; // = 92.4986;
extern double pk_21; // = 87.3071;
extern double pk_20; // = 82.4069;
extern double pk_19; // = 77.7817;
extern double pk_18; // = 73.4162;
extern double pk_17; // = 69.2957;
extern double pk_16; // = 65.4064;
extern double pk_15; // = 61.7354;
extern double pk_14; // = 58.2705;
extern double pk_13; // = 55.0;
extern double pk_12; // = 51.9131;
extern double pk_11; // = 48.9994;
extern double pk_10; // = 46.2493;
extern double pk_9	; // = 43.6535;
extern double pk_8	; // = 41.2034;
extern double pk_7	; // = 38.8909;
extern double pk_6	; // = 36.7081;
extern double pk_5	; // = 34.6478;
extern double pk_4	; // = 32.7032;
extern double pk_3	; // = 30.8677;
extern double pk_2	; // = 29.1352;
extern double pk_1	; // = 27.5; //first key on left-hand side


//void PKTap(Voice &v, double pk, double volume); //press-release=short fade
//void PKHold(Voice &v, double pk, double volume); //hold-pressed=long fade

void TwinkleStar(Voice &v);
void TwinkleStar_Treble(Voice &v);
void AmaGrace(Voice &v);
void OwnCompo(Voice &v);
void StepAllKeys(Voice &v);
void FourChords(Voice &v);


#endif
