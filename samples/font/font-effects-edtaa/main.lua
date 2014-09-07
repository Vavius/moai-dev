----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_GLYPHS, 1, 0, 0, 1, 1 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

onGlyph = function ( font, reader, image, code, x, y, xMin, yMin, xMax, yMax )

	print ( 'GLYPH:', font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	reader:renderGlyph ( image, x, y )
	image:generateSDFEdtaa (xMin - 10, yMin - 10, xMax + 10, yMax + 10, 0.1)
	
end

file = assert ( io.open ( 'MOAIFontEffectsShader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'MOAIFontEffectsShader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'smoothFactor', MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 2, 'scale', MOAIShaderProgram.UNIFORM_FLOAT )

program:load ( vsh, fsh )

color = MOAIColor.new ()
color:setColor ( 1, 1, 1, 1 )

shader = MOAIShader.new ()
shader:setProgram ( program )
shader:setAttr ( 1, 0.1 )

charcodes = 'ABCQWERTYUIOP{ASDFGHJKL:ZXCVBNM<qwertyuiopasdfghjklzxcvbnm,1234567890'

font = MOAIFont.new ()
font:setListener ( MOAIFont.EVENT_RENDER_GLYPH, onGlyph )

cache = font:getCache ()
cache:setPadding ( 20, 20 )

font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 25 )

coro = MOAICoroutine.new()
coro:run(function()
    coroutine.yield()
    image = font:getImage ()
    image:writePNG ( 'font.png' )
end)

-- font now ready for use
textbox = MOAITextBox.new ()
textbox:setText ( 'Smooth scaling SDF text' )
textbox:setFont ( font )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setShader ( shader )
textbox:setAlignment(MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY)
textbox:setScl(0.2, 0.2, 1)
textbox:seekScl(10, 10, 1, 6, MOAIEaseType.EASE_OUT):setMode(MOAITimer.PING_PONG)
shader:setAttrLink(2, textbox, MOAITransform.ATTR_X_SCL)

style = textbox:getStyle ()
style:setPadding ( 20, 20 )

layer:insertProp ( textbox )
