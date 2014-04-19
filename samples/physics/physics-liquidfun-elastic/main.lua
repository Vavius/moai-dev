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
UNITS_TO_METERS = 0.03

world = MOAIBox2DWorld.new ()
world:setUnitsToMeters ( UNITS_TO_METERS )
world:setGravity ( 0, -10 / UNITS_TO_METERS )
world:start ()
layer:setBox2DWorld ( world )

worldBody = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = worldBody:addRect ( -80, -100, 80, -150)

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( 'moai.png' )
texture:setRect ( -25/2, -25/2, 25/2, 25/2 )

particleSystem = world:addParticleSystem ()
particleSystem:setRadius ( 4 )
particleSystem:setDensity ( 1 )

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

function randomShape ()
    local r = math.random()

    local shape = MOAIBox2DShape.new ()
    if r < 0.5 then
        shape:initCircle ( 0, 0, math.random(15, 30), UNITS_TO_METERS )
    else
        shape:initRect ( math.random(-30, -10), math.random(-20, -10), 
            math.random(10, 30), math.random(10, 20), 0, UNITS_TO_METERS )
    end
    return shape
end


function addElasticGroup (x, y)
    -- def.flags               = state.GetField < u32 >( 2, "flags", 0 );
    -- def.groupFlags          = state.GetField < u32 >( 2, "groupFlags", 0 );
    -- def.position.x          = state.GetField < float >( 2, "x", 0.0f );
    -- def.position.y          = state.GetField < float >( 2, "y", 0.0f );
    -- def.angle               = state.GetField < float >( 2, "angle", 0.0f );
    -- def.linearVelocity.x    = state.GetField < float >( 2, "velocityX", 0.0f );
    -- def.linearVelocity.y    = state.GetField < float >( 2, "velocityY", 0.0f );
    -- def.angularVelocity     = state.GetField < float >( 2, "angularVelocity", 0.0f );
    -- def.strength            = state.GetField < float >( 2, "strength", 1.0f );
    -- def.stride              = state.GetField < float >( 2, "stride", 0.0f );
    -- def.lifetime            = state.GetField < float > ( 2, "lifetime", 0.0f );
    -- def.particleCount       = state.GetField < u32 >( 2, "particleCount", 0 );

    local shape = randomShape ()

    group = particleSystem:createParticleGroup {
        flags = MOAIBox2DParticle.ELASTIC + MOAIBox2DParticle.REPULSIVE,
        shape = shape,
        x = x,
        y = y,
    }
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
            addElasticGroup ( worldX, worldY )
        end
	else
		if pick then
			--also destroys joint
			mouseBody:destroy()
			mouseBody = nil
			pick = nil
        else
            -- pass
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
