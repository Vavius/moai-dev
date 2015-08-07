----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

function makeCurve ( ease )

	local curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 2 )
	curve:setKey ( 1, 0, 0, ease )
	curve:setKey ( 2, 256, 256 )
	
	return curve
end

local curves = {
	makeCurve ( MOAIEaseType.EASE_IN ),
	makeCurve ( MOAIEaseType.EASE_OUT ),
	makeCurve ( MOAIEaseType.EXTRA_SHARP_EASE_IN ),
	makeCurve ( MOAIEaseType.EXTRA_SHARP_EASE_OUT ),
	makeCurve ( MOAIEaseType.EXTRA_SHARP_SMOOTH ),
	makeCurve ( MOAIEaseType.EXTRA_SHARP_SMOOTH_EASE_OUT ),
	makeCurve ( MOAIEaseType.FLAT ),
	makeCurve ( MOAIEaseType.LINEAR ),
	makeCurve ( MOAIEaseType.SHARP_EASE_IN ),
	makeCurve ( MOAIEaseType.SHARP_EASE_OUT ),
	makeCurve ( MOAIEaseType.SHARP_SMOOTH ),
	makeCurve ( MOAIEaseType.SHARP_SMOOTH_EASE_OUT ),
	makeCurve ( MOAIEaseType.SMOOTH ),
	makeCurve ( MOAIEaseType.SMOOTH_EASE_OUT ),
	makeCurve ( MOAIEaseType.SOFT_EASE_IN ),
	makeCurve ( MOAIEaseType.SOFT_EASE_OUT ),
	makeCurve ( MOAIEaseType.SOFT_SMOOTH ),
	makeCurve ( MOAIEaseType.SOFT_SMOOTH_EASE_OUT ),
	makeCurve ( MOAIEaseType.SINE_EASE_IN ),
	makeCurve ( MOAIEaseType.SINE_EASE_OUT ),
	makeCurve ( MOAIEaseType.SINE_SMOOTH ),
	makeCurve ( MOAIEaseType.CIRC_EASE_IN ),
	makeCurve ( MOAIEaseType.CIRC_EASE_OUT ),
	makeCurve ( MOAIEaseType.CIRC_SMOOTH ),
	makeCurve ( MOAIEaseType.BOUNCE_IN ),
	makeCurve ( MOAIEaseType.BOUNCE_OUT ),
	makeCurve ( MOAIEaseType.BOUNCE_SMOOTH ),
	makeCurve ( MOAIEaseType.ELASTIC_IN ),
	makeCurve ( MOAIEaseType.ELASTIC_OUT ),
	makeCurve ( MOAIEaseType.ELASTIC_SMOOTH ),
	makeCurve ( MOAIEaseType.BACK_EASE_IN ),
	makeCurve ( MOAIEaseType.BACK_EASE_OUT ),
	makeCurve ( MOAIEaseType.BACK_EASE_SMOOTH ),
}

local names = {
	'EASE_IN',
	'EASE_OUT',
	'EXTRA_SHARP_EASE_IN',
	'EXTRA_SHARP_EASE_OUT',
	'EXTRA_SHARP_SMOOTH',
	'EXTRA_SHARP_SMOOTH_EASE_OUT',
	'FLAT',
	'LINEAR',
	'SHARP_EASE_IN',
	'SHARP_EASE_OUT',
	'SHARP_SMOOTH',
	'SHARP_SMOOTH_EASE_OUT',
	'SMOOTH',
	'SMOOTH_EASE_OUT',
	'SOFT_EASE_IN',
	'SOFT_EASE_OUT',
	'SOFT_SMOOTH',
	'SOFT_SMOOTH_EASE_OUT',
	'SINE_EASE_IN',
	'SINE_EASE_OUT',
	'SINE_SMOOTH',
	'CIRC_EASE_IN',
	'CIRC_EASE_OUT',
	'CIRC_SMOOTH',
	'BOUNCE_IN',
	'BOUNCE_OUT',
	'BOUNCE_SMOOTH',
	'ELASTIC_IN',
	'ELASTIC_OUT',
	'ELASTIC_SMOOTH',
	'BACK_EASE_IN',
	'BACK_EASE_OUT',
	'BACK_EASE_SMOOTH',
}

local prevID = 0
local curveID = 1

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.drawAnimCurve ( curves [ curveID ], 100 )
	if prevID ~= curveID then
		print ( names [ curveID ])
		prevID = curveID
	end
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -128, -128, 128, 128 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
prop:setLoc ( -128, -128 )
layer:insertProp ( prop )

function onKeyboardEvent ( key, down )

	if down == true then
		curveID = curveID + 1
		if #curves < curveID then curveID = 1 end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

