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
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setPartition( MOAIPartition.new() )
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setUnitsToMeters ( .05 )
world:start ()
layer:setBox2DWorld ( world )

math.randomseed(os.time())
function addBody ()
	local body = world:addBody ( MOAIBox2DBody.KINEMATIC )

    local w, h = 250, 20
	local fixture = body:addRect (-0.5 * w, -0.5 * h, 0.5 * w, 0.5 * h, math.random(0, 360))

    local x = math.random(100, 320)
    local y = math.random(80, 240)
    local xSign = math.random() < 0.5 and -1 or 1
    local ySign = math.random() < 0.5 and -1 or 1
    body:setTransform ( x * xSign, y * ySign )
	body:setAngularVelocity ( 8 * (math.random() - 0.5) )
end

for i = 1, 10 do
    addBody()
end


points = {}
normals = {}

-- not accurate reclections (here I use normals as reflection directions)
function emitRay ( iterations )
    local x, y = points [ #points - 1 ], points [ #points ]
    local xn, yn = normals [ #normals - 1 ], normals [ #normals ]

    x, y, xn, yn = world:rayCast ( x, y, x + xn * 400, y + yn * 400,
        MOAIBox2DWorld.RAYCAST_NEAREST, MOAIBox2DWorld.RAYCAST_POINT + MOAIBox2DWorld.RAYCAST_NORMAL )

    if iterations > 0 and x then
        points [ #points + 1 ] = x
        points [ #points + 1 ] = y

        normals [ #normals + 1 ] = xn
        normals [ #normals + 1 ] = yn

        emitRay ( iterations - 1 )
    end
end


function draw ( index, xOff, yOff, xFlip, yFlip )
    MOAIDraw.drawLine ( points )

    local c = #points
    local x, y = points [ c - 1 ], points [ c ]
    MOAIDraw.drawLine ( x, y, x + 400 * normals [ c - 1 ], y + 400 * normals [ c ] )
end

deck = MOAIScriptDeck.new ()
deck:setRect ( -320, -240, 320, 240 )
deck:setDrawCallback ( draw )

prop = MOAIProp.new ()
prop:setDeck ( deck )

layer:insertProp ( prop )

thread = MOAICoroutine.new ()
thread:run ( function ()
    local angle = math.random()
    while true do
        angle = angle + 0.001
        points = { 0, 0 }
        normals = { math.cos(angle), math.sin(angle) }
        emitRay ( 10 )
        coroutine.yield()
    end
end)



