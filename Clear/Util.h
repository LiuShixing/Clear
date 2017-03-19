
#ifndef UTIL_H
#define UTIL_H

const float OBJ_SCALE = 1.0f;

const float WIDTH = 1.0f*OBJ_SCALE;
const float LENGHT = 3.0f*OBJ_SCALE;


const float G = 80.0f;
const float MAX_SPEED = 30.0f*OBJ_SCALE;
const float ZERO = 0.000001f;


const float CAMERA_OFFSET_X = 30.f;
const float MAP_PLAYER_HEIGHT = floor(CAMERA_OFFSET_X*tanf(0.25f*XM_PI*0.5f)*2.0f);
const int MAX_LAYER_COUNT = 3;

const float MAP_UNIT_LENGHT = floor(MAP_PLAYER_HEIGHT / (9.0f / 16.0f));
const float MAX_MAP_LENGHT = 3.0f*MAP_UNIT_LENGHT;

const float CAMERA_MIN_Z = MAP_UNIT_LENGHT*0.5f;
const float CAMERA_MAX_Z = MAX_MAP_LENGHT - MAP_UNIT_LENGHT*0.5f;

const float MAP_LEFT = 0.0f;
const float MAP_RIGHT = MAP_LEFT + MAX_MAP_LENGHT;
const float MAP_BUTTOM = 0.0f;
const float MAP_TOP = MAP_BUTTOM + MAX_LAYER_COUNT*MAP_PLAYER_HEIGHT;

const float ZOM_ALERT_DISTANCE = MAP_UNIT_LENGHT*0.4f;
const float ZOM_ATTACK_DISTANCE = WIDTH*4.0f;

const float LEONA_CUT_VALID_DISTANCE = WIDTH*6.0f;
const float LEONA_CHI_VALID_DISTANCE = WIDTH*10.0f;

#endif