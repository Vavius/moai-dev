----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

text = 'This is a label.'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF' )

label = MOAITextLabel.new ()
label:setString ( text )
label:setFont ( font )
label:setTextSize ( 32 )
label:setYFlip ( true )
label:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.BASELINE_JUSTIFY )
layer:insertProp ( label )

nd1 = MOAIScriptNode.new ()
nd1:reserveAttrs ( 1 )
nd1:setAttrName ( 1, "text" )

label:setAttrLink ( MOAITextBox.ATTR_TEXT, nd1, 1 )

coro = MOAICoroutine.new ()
coro:run(function()
	for i = 1, 20 do
		nd1.text = "Elapsed frames: " .. i * 10
		for j = 1, 10 do
			coroutine.yield()
		end
	end
end)

