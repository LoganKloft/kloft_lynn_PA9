#pragma once
/*
*	Based off: https://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array.php
*/

#include <iostream>
#include <fstream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/*
*   tiles has width 23 and height 17 for total indicies 23*17 = 391
* 
*/

#define MAX_LEVEL_WIDTH 22
#define MAX_LEVEL_HEIGHT 16

// Tileset textures
#define TILE_VERTICAL 3
#define TILE_HORIZONTAL 2
#define TILE_TOPL_CORNER 8
#define TILE_TOPR_CORNER 9
#define TILE_BOTTOMR_CORNER 10
#define TILE_BOTTOML_CORNER 11
#define TILE_GRASS 18

class Level : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const std::string& tileFiles, unsigned int width, unsigned int height)
    {
        // set up tiles array
        if (tiles != nullptr) delete[] tiles;
        tiles = new int[width * height];
        for (int i = 0; i < width * height; i++) tiles[i] = 18;
        
        // open file to load into tiles array
        if(tileFiles != "")
        {
            std::ifstream infile(tileFiles);
            if (!infile.is_open()) return false;
            std::string digit = "";

            int i = 0, j = 0, count = 0;
            while (std::getline(infile, digit, ','))
            {
                tiles[i + j * width] = stoi(digit);
                std::cout << tiles[i + j * width] << " ";
                i++;
                if (i == width)
                {
                    std::cout << std::endl;
                    i = 0;
                    j++;
                }
                count++;
                if (count == width * height) break;
            }
            infile.close();
        }

        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

    // Only call after making a single call to the load function - m_tileset must be initialized
    void setTile(sf::Vector2u tileSize, int x, int y, int width, int tileNumber)
    {
        if (x < 0 || x > MAX_LEVEL_WIDTH) return;
        if (y < 0 || y > MAX_LEVEL_HEIGHT) return;

        // update tiles array
        tiles[x + y * width] = tileNumber;

        // position in tileset texture
        int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
        int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

        // pointer to current tile's quad
        sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

        // define quad's 4 texture coordinates
        quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
    }

    void saveTile(const std::string& newTileFile, unsigned int width, unsigned int height)
    {
        std::ofstream outfile(newTileFile);
        if (!outfile.is_open()) return;
        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                outfile << tiles[j + i * width] << ',';
            }
            outfile << std::endl;
        }
        outfile.close();
    }

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform(); // apply the transform
		states.texture = &m_tileset; // apply the tileset texture
		target.draw(m_vertices, states); // draw the vertex array
	}
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
    int* tiles;
};