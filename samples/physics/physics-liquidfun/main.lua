----------------------------------------------------------------
-- Copyright (c) 2010-2013 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local width = MOAIEnvironment.horizontalResolution or 640
local height = MOAIEnvironment.verticalResolution or 480
MOAISim.openWindow ( "test", width ,height  )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( 320, 240 )

layer = MOAILayer2D.new ()
layer:setPartition( MOAIPartition.new() )
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setUnitsToMeters ( 0.03 )
world:setGravity ( 0, -10 / 0.03 )
world:start ()
layer:setBox2DWorld ( world )

worldBody = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = worldBody:addRect ( -80, -100, 80, -150)
fixture2:setFilter ( 0x02 )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( 'moai.png' )
texture:setRect ( -25/2, -25/2, 25/2, 25/2 )

particleSystem = world:addParticleSystem () 

particleSystem:setRadius ( 4 )
particleSystem:setDensity ( 1 )
particleSystem:setMaxParticleCount ( 1000 )

function addSprite()
	local body = world:addBody ( MOAIBox2DBody.DYNAMIC, worldX, worldY )

	local poly = {
		25/2, -25/2,
		25/2, 25/2,
		-25/2, 25/2,
		-25/2, -25/2,
	}

	local fixture = body:addPolygon ( poly )
	fixture:setDensity ( 1 )
	fixture:setFriction ( 0.3 )
	fixture:setFilter ( 0x01 )

	body:resetMassData ()

	local sprite = MOAIProp2D.new ()
	sprite:setDeck ( texture )
	sprite.body = body
	sprite:setParent ( body )
	layer:insertProp ( sprite )
end


-- all possible particle flags:
--    WATER
--    ZOMBIE
--    WALL
--    SPRING
--    ELASTIC
--    VISCOUS
--    POWDER
--    TENSILE
--    COLOR_MIXING
--    DESTRUCTION_LISTENER
--    BARRIER
--    STATIC_PRESSURE
--    REACTIVE
--    REPULSIVE
--    FIXTURE_CONTACT_LISTENER
--    PARTICLE_CONTACT_LISTENER
--    FIXTURE_CONTACT_FILTER
--    PARTICLE_CONTACT_FILTER
flags = MOAIBox2DParticle.COLOR_MIXING + MOAIBox2DParticle.TENSILE + MOAIBox2DParticle.VISCOUS

colors = { 
    { 1, 0.5, 0.5 },
    { 0.5, 1, 0.5 },
    { 0.5, 0.5, 1 }
}
color = colors [ 1 ]

function addParticle ( count )
    count = count or 1
    for i = 1, count do
        -- randomize positions slightly to prevent numerical blow
        particleSystem:createParticle ( flags, worldX + math.random(-20, 20), worldY + math.random(-20, 20), 
            0, 0,   -- velocity
            0,      -- lifetime (0 = infinity)
            color[1], color[2], color[3] )
    end
end


function wait ( t )
    while t > 0 do
        t = t - coroutine.yield ()
    end
end

toggleColors = MOAICoroutine.new ()
toggleColors:run ( function()
    while true do
        color = colors [ math.random(1, 3) ]
        wait ( 10 )
    end
end)

particleEmmisionTread = MOAICoroutine.new ()
function startEmission ()
    particleEmmisionTread:run ( function () 
        while true do
            addParticle ( 2 )
            coroutine.yield ()
        end
    end)
end

function stopEmission ()
    particleEmmisionTread:stop()
end

function pointerCallback ( x, y )
	worldX, worldY = layer:wndToWorld ( x, y )

	if pick then
        mouseJoint:setTarget(worldX, worldY)
	end
end

function clickCallback ( down )
	if down then
		pick = layer:getPartition():propForPoint ( worldX, worldY)
		if pick then
			mouseBody = world:addBody( MOAIBox2DBody.DYNAMIC )
			mouseBody:setTransform(worldX, worldY)

			mouseJoint = world:addMouseJoint(mouseBody, pick.body, worldX, worldY,  10000.0 * pick.body:getMass());
			mouseJoint:setDampingRatio(2);
		else
            startEmission ()
        end
	else
		if pick then
			--also destroys joint
			mouseBody:destroy()
			mouseBody = nil
			pick = nil
        else
            stopEmission()
		end
	end
end

function rightclickCallback ( down )
	if down then
        addSprite()
	end
end


if MOAIInputMgr.device.pointer then
    MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
    MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
    MOAIInputMgr.device.mouseRight:setCallback ( rightclickCallback )
else
    MOAIInputMgr.device.touch:setCallback (
        function ( eventType, idx, x, y, tapCount )
            local isDown = eventType == MOAITouchSensor.TOUCH_DOWN
            if (tapCount > 1) then
                rightclickCallback(isDown)
            elseif eventType ~= MOAITouchSensor.TOUCH_MOVE then
                pointerCallback(x,y)
                clickCallback(isDown)
            elseif eventType == MOAITouchSensor.TOUCH_MOVE then
                pointerCallback(x,y)
            end
        end
    )
end
