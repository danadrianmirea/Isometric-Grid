#pragma once

#include<algorithm>
#include<array>
#include<cmath>
#include<functional>
#include<numeric>
#include<vector>

#include"raylib.h"

#include"constants.h"
#include"grid.h"
#include"storage.h"

struct Button {

   Texture2D& texture;
   Rectangle source{};
   Rectangle bounds{};

   int state{0}; // 0 normal, 1 hover, 2 click

   // Top, middle or bottom of menu
   sectionType type;

   // buttion function on click
   std::function<void(Grid&)> onClick;

   // grid to run onClick on
   Grid& grid;

   // button held duration
   int heldDuration{0};

   Button(const char* filename, sectionType type, float x, float y, float x_div, float y_div, float scale, std::function<void(Grid&)> onClick, Grid& grid):
      texture(textureStore.add(filename)), type(type), onClick(onClick), grid(grid){

         // set source
         source.width  = texture.width/x_div;
         source.height = texture.height/y_div;
         source.y = source.height*static_cast<int>(type);

         // set bounds
         bounds = {x,y,source.width*scale,source.height*scale};
   };

   void display(const Vector2& mousePos);
};

void Button::display(const Vector2& mousePos){

   if (CheckCollisionPointRec(mousePos, bounds)){

      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ){
         state = 2;
         heldDuration++;
         if (heldDuration >= 15){ onClick(grid); }
      }
      else { state = 1; }

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
         onClick(grid);
         heldDuration = 0;
      }
   }
   else { state = 0; }

   source.x = state*texture.width/3;
   
   DrawTexturePro(texture, source, bounds, {}, 0.0f, WHITE);
}

