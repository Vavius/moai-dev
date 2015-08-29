----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

xScale = 0.5
yScale = 0.5

srcImage = MOAIImage.new ()
srcImage:load ( "img.png" )

w0, h0 = srcImage:getSize ()

w1 = w0 * xScale
h1 = h0 * yScale

image = MOAIImage.new ()
image:init ( w1, h1 )

image:copyRect ( srcImage,
	0, 0, w0, h0,
	0, 0, w1, h1
)
image:writePNG ( 'small.png' )

local pixels = {}
for y = 0, h1 - 1 do
    pixels [ y ] = {}
    for x = 0, w1 - 1 do
        pixels [ y ][ x ] = { image:getRGBA ( x, y )}
    end
end

img2 = MOAIImage.new ()
imgGray = MOAIImage.new ()
img2:init ( w1, h1 )
imgGray:init ( w1, h1, MOAIImage.COLOR_FMT_A_8 )
for y = 0, h1 - 1 do
    for x = 0, w1 - 1 do
        local r, g, b, a = unpack ( pixels [ y ][ x ])
        img2:setRGBA ( x, y, r, g, b, 1 )
        imgGray:setRGBA ( x, y, 0, 0, 0, a )
    end
end
img2:writePNG ( 'color.png' )
imgGray:writePNG ( 'gray.png' )


MOAISim.openWindow ( "test", w1, h1 )

viewport = MOAIViewport.new ()
viewport:setSize ( w1, h1 )
viewport:setScale ( w1, -h1 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

hw1 = w1 * 0.5
hh1 = h1 * 0.5

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( image )
gfxQuad:setRect ( -hw1, -hh1, hw1, hh1 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

