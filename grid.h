#pragma once

#include<vector>

#include"raylib.h"
#include"raymath.h"

#include"constants.h"
#include"textures.h"
#include"wave.h"

struct Grid{

   int width;
   int height;

   Texture2D texture{LoadTexture("sprites/tile.png")};
   Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
   
   std::vector<Rectangle> tiles;

   int lastHovered{-1};
   int tileRise{-10};

   Grid(int width, int height);

   void mouseHover(const Vector2& mousePos);
   void draw();
};

Grid::Grid(int width, int height): width(width), height(height){

   // add tile texture to store for easier unloading
   textureStore.add(texture);

   // create grid with initial positions
   Vector3 temp;
   for (int j=0; j<height; j++){
      for (int i=0; i<width; i++){
         temp = Vector3Transform({static_cast<float>(i)*scale, static_cast<float>(j)*scale, 1}, toIso);

         tiles.push_back({temp.x,temp.y,scale,scale});
      }
   }
}

void Grid::mouseHover(const Vector2& mousePos){

   // undo last hover
   if (lastHovered>=0){ tiles[lastHovered].y -= tileRise; }

   // check for mouse hover
   Vector3 onGrid = Vector3Transform({mousePos.x, mousePos.y, 1}, toGrid);
   float x_pos = onGrid.x/scale;
   float y_pos = onGrid.y/scale;

   // make sure mouse within grid boundaries
   if ( x_pos >= 0.0f && x_pos < width && y_pos >= 0.0f && y_pos < height){
      lastHovered = static_cast<int>(x_pos) + static_cast<int>(y_pos)*width;
      tiles[lastHovered].y += tileRise;
   }
   else {lastHovered = -1;}

   // debug info
   // DrawText(TextFormat("Mouse Actual Pos: %04.00f, %04.00f", mousePos.x, mousePos.y), 0,0,20, BLACK);
   // DrawText(TextFormat("Mouse Isomet Pos: %04.00f, %04.00f", onGrid.x, onGrid.y), 0,25,20, BLACK);
   DrawText(TextFormat("Mouse Coord:   %i, %i", static_cast<int>(onGrid.x/scale), (static_cast<int>(onGrid.y/scale))), 0,0,20, BLACK);
   DrawText(TextFormat("Index:   %i", lastHovered), 0,25,20, BLACK);
}

void Grid::draw(){

   for (const auto& tile : tiles){
      DrawTexturePro(texture, source, {tile.x, tile.y+waveOffset(tile), tile.width, tile.height}, {}, 0.0f, WHITE);
   }
}