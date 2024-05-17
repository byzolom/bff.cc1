--[[
	WARNING: Heads up! This script has not been verified by ScriptBlox. Use at your own risk!
]]
notificationGui = Instance.new("ScreenGui")
notificationGui.Name = "NotificationGui"
notificationGui.Parent = game.Players.LocalPlayer.PlayerGui
local notificationFrame = Instance.new("Frame")
notificationFrame.Size = UDim2.new(0, 200, 0, 50)
notificationFrame.Position = UDim2.new(0, 10, 1, -60)
notificationFrame.AnchorPoint = Vector2.new(0, 1)
notificationFrame.BackgroundColor3 = Color3.new(0, 0, 0)
notificationFrame.BackgroundTransparency = 0.0
notificationFrame.BorderSizePixel = 1
notificationFrame.BorderColor3 = Color3.new(1, 1, 1)
notificationFrame.Parent = notificationGui
local notificationText = Instance.new("TextLabel")
notificationText.Size = UDim2.new(1, -10, 1, -10)
notificationText.Position = UDim2.new(0, 5, 0, 5)
notificationText.BackgroundTransparency = 1
notificationText.Text = "Aquuas  Head Hitbox+ESP+Name loaded run it"
notificationText.TextColor3 = Color3.new(1, 1, 1)
notificationText.TextSize = 14
notificationText.Font = Enum.Font.SourceSansBold
notificationText.TextWrapped = true
notificationText.Parent = notificationFrame

local NotificationDuration = 3
local function RemoveNotification()
    notificationGui:Destroy()
end
wait(NotificationDuration)
RemoveNotification()

local BoxESP = {}
function BoxESP.Create(Player)
    local Box = Drawing.new("Square")
    Box.Visible = false
    Box.Color = Color3.fromRGB(194, 17, 17)
    Box.Filled = false
    Box.Transparency = 0.50
    Box.Thickness = 3

    local Updater

    local function UpdateBox()
        if Player and Player:IsA("Model") and Player:FindFirstChild("HumanoidRootPart") and Player:FindFirstChild("Head") then
            local Target2dPosition, IsVisible = workspace.CurrentCamera:WorldToViewportPoint(Player.HumanoidRootPart.Position)
            local scale_factor = 0 / (Target2dPosition.Z * math.tan(math.rad(workspace.CurrentCamera.FieldOfView * 0.0)) * 0) * 0
            local width, height = math.floor(0 * scale_factor), math.floor(0 * scale_factor)

            Box.Visible = IsVisible
            Box.Size = Vector2.new(width, height)
            Box.Position = Vector2.new(Target2dPosition.X - Box.Size.X / 2, Target2dPosition.Y - Box.Size.Y / 2)
        else
            Box.Visible = false
            if not Player then
                Box:Remove()
                Updater:Disconnect()
            end
        end
    end

    Updater = game:GetService("RunService").RenderStepped:Connect(UpdateBox)

    return Box
end

local Boxes = {}

local function EnableBoxESP()
    for _, Player in pairs(game:GetService("Workspace"):GetChildren()) do
        if Player:IsA("Model") and Player:FindFirstChild("HumanoidRootPart") and Player:FindFirstChild("Head") then
            local Box = BoxESP.Create(Player)
            table.insert(Boxes, Box)
        end
    end
end

game.Workspace.DescendantAdded:Connect(function(i)
    if i:IsA("Model") and i:FindFirstChild("HumanoidRootPart") and i:FindFirstChild("Head") then
        local Box = BoxESP.Create(i)
        table.insert(Boxes, Box)
    end
end)

EnableBoxESP()

local SelectPart = "Head"
local HBSizeX = 10
local HBSizeY = 10
local HBSizeZ = 10
local HBTrans = 1.0

local PurpleColor = BrickColor.new("Bright red")

local hitboxlist = {}

task.spawn(function ()
    while wait() do
        for v, i in pairs(workspace:GetChildren()) do
            if i:FindFirstChild("HumanoidRootPart") and not i:FindFirstChild("Fake") then
                local FakeHead = Instance.new("Part", i)
                FakeHead.CFrame = i.HumanoidRootPart.CFrame
                FakeHead.Name = SelectPart
                FakeHead.Size = Vector3.new(HBSizeX, HBSizeY, HBSizeZ)
                FakeHead.Anchored = true
                FakeHead.CanCollide = false
                FakeHead.Transparency = HBTrans
                FakeHead.BrickColor = PurpleColor  -- Apply the purple color to the hitbox part
                local subndom = Instance.new("Part", i)
                subndom.Name = "Fake"
                table.insert(hitboxlist, FakeHead)
                table.insert(hitboxlist, subndom)
            end
        end
    end
end)

print("Hitbox script initialized.")



function sandbox(var,func)
local env = getfenv(func)
local newenv = setmetatable({},{
__index = function(self,k)
if k=="script" then
return var
else
return env[k]
end
end,
})
setfenv(func,newenv)
return func
end
cors = {}
mas = Instance.new("Model",game:GetService("Lighting"))
LocalScript0 = Instance.new("LocalScript")
LocalScript0.Name = "FreeCamera"
LocalScript0.Parent = mas
table.insert(cors,sandbox(LocalScript0,function()
local pi    = math.pi
local abs   = math.abs
local clamp = math.clamp
local exp   = math.exp
local rad   = math.rad
local sign  = math.sign
local sqrt  = math.sqrt
local tan   = math.tan
 
local ContextActionService = game:GetService("ContextActionService")
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local StarterGui = game:GetService("StarterGui")
local UserInputService = game:GetService("UserInputService")
 
local LocalPlayer = Players.LocalPlayer
if not LocalPlayer then
Players:GetPropertyChangedSignal("LocalPlayer"):Wait()
LocalPlayer = Players.LocalPlayer
end
 
local Camera = workspace.CurrentCamera
workspace:GetPropertyChangedSignal("CurrentCamera"):Connect(function()
local newCamera = workspace.CurrentCamera
if newCamera then
Camera = newCamera
end
end)
local TOGGLE_INPUT_PRIORITY = Enum.ContextActionPriority.Low.Value
local INPUT_PRIORITY = Enum.ContextActionPriority.High.Value
local FREECAM_MACRO_KB = {Enum.KeyCode.LeftShift, Enum.KeyCode.P}
 
local NAV_GAIN = Vector3.new(2, 3, 2)*84
local PAN_GAIN = Vector2.new(0.85, 1)*9
local FOV_GAIN = 700
 
local PITCH_LIMIT = rad(90)
 
local VEL_STIFFNESS = 19.5
local PAN_STIFFNESS = 15.0
local FOV_STIFFNESS = 10.0
local Spring = {} do
Spring.__index = Spring
 
function Spring.new(freq, pos)
local self = setmetatable({}, Spring)
self.f = freq
self.p = pos
self.v = pos*0
return self
end
 
function Spring:Update(dt, goal)
local f = self.f*2*pi
local p0 = self.p
local v0 = self.v
 
local offset = goal - p0
local decay = exp(-f*dt)
 
local p1 = goal + (v0*dt - offset*(f*dt + 1))*decay
local v1 = (f*dt*(offset*f - v0) + v0)*decay
 
self.p = p1
self.v = v1
 
return p1
end
 
function Spring:Reset(pos)
self.p = pos
self.v = pos*0
end
end
 

 
local cameraPos = Vector3.new()
local cameraRot = Vector2.new()
local cameraFov = 1
 
local velSpring = Spring.new(VEL_STIFFNESS, Vector3.new())
local panSpring = Spring.new(PAN_STIFFNESS, Vector2.new())
local fovSpring = Spring.new(FOV_STIFFNESS, 0)
 

local Input = {} do
local thumbstickCurve do
local K_CURVATURE = 2.0
local K_DEADZONE = 0.15
 
local function fCurve(x)
return (exp(K_CURVATURE*x) - 1)/(exp(K_CURVATURE) - 1)
end
 
local function fDeadzone(x)
return fCurve((x - K_DEADZONE)/(1 - K_DEADZONE))
end
 
function thumbstickCurve(x)
return sign(x)*clamp(fDeadzone(abs(x)), 0, 1)
end
end
 
local gamepad = {
ButtonX = 0,
ButtonY = 0,
DPadDown = 0,
DPadUp = 0,
ButtonL2 = 0,
ButtonR2 = 0,
Thumbstick1 = Vector2.new(),
Thumbstick2 = Vector2.new(),
}
 
local keyboard = {
W = 0,
A = 0,
S = 0,
D = 0,
E = 0,
Q = 0,
U = 0,
H = 0,
J = 0,
K = 0,
I = 0,
Y = 0,
Up = 0,
Down = 0,
LeftShift = 0,
RightShift = 0,
}
 
local mouse = {
Delta = Vector2.new(),
MouseWheel = 0,
}
 
local NAV_GAMEPAD_SPEED  = Vector3.new(1, 1, 1)
local NAV_KEYBOARD_SPEED = Vector3.new(1, 1, 1)
local PAN_MOUSE_SPEED    = Vector2.new(1, 1)*(pi/64)
local PAN_GAMEPAD_SPEED  = Vector2.new(1, 1)*(pi/8)
local FOV_WHEEL_SPEED    = 1.0
local FOV_GAMEPAD_SPEED  = 0.25
local NAV_ADJ_SPEED      = 0.75
local NAV_SHIFT_MUL      = 0.25
 
local navSpeed = 1
 
function Input.Vel(dt)
navSpeed = clamp(navSpeed + dt*(keyboard.Up - keyboard.Down)*NAV_ADJ_SPEED, 0.01, 4)
 
local kGamepad = Vector3.new(
thumbstickCurve(gamepad.Thumbstick1.x),
thumbstickCurve(gamepad.ButtonR2) - thumbstickCurve(gamepad.ButtonL2),
thumbstickCurve(-gamepad.Thumbstick1.y)
)*NAV_GAMEPAD_SPEED
 
local kKeyboard = Vector3.new(
keyboard.D - keyboard.A + keyboard.K - keyboard.H,
keyboard.E - keyboard.Q + keyboard.I - keyboard.Y,
keyboard.S - keyboard.W + keyboard.J - keyboard.U
)*NAV_KEYBOARD_SPEED
 
local shift = UserInputService:IsKeyDown(Enum.KeyCode.LeftShift) or UserInputService:IsKeyDown(Enum.KeyCode.RightShift)
 
return (kGamepad + kKeyboard)*(navSpeed*(shift and NAV_SHIFT_MUL or 1))
end
 
function Input.Pan(dt)
local kGamepad = Vector2.new(
thumbstickCurve(gamepad.Thumbstick2.y),
thumbstickCurve(-gamepad.Thumbstick2.x)
)*PAN_GAMEPAD_SPEED
local kMouse = mouse.Delta*PAN_MOUSE_SPEED
mouse.Delta = Vector2.new()
return kGamepad + kMouse
end
 
function Input.Fov(dt)
local kGamepad = (gamepad.ButtonX - gamepad.ButtonY)*FOV_GAMEPAD_SPEED
local kMouse = mouse.MouseWheel*FOV_WHEEL_SPEED
mouse.MouseWheel = 0
return kGamepad + kMouse
end
 
do
local function Keypress(action, state, input)
keyboard[input.KeyCode.Name] = state == Enum.UserInputState.Begin and 1 or 0
return Enum.ContextActionResult.Sink
end
 
local function GpButton(action, state, input)
gamepad[input.KeyCode.Name] = state == Enum.UserInputState.Begin and 1 or 0
return Enum.ContextActionResult.Sink
end
 
local function MousePan(action, state, input)
local delta = input.Delta
mouse.Delta = Vector2.new(-delta.y, -delta.x)
return Enum.ContextActionResult.Sink
end
 
local function Thumb(action, state, input)
gamepad[input.KeyCode.Name] = input.Position
return Enum.ContextActionResult.Sink
end
 
local function Trigger(action, state, input)
gamepad[input.KeyCode.Name] = input.Position.z
return Enum.ContextActionResult.Sink
end
 
local function MouseWheel(action, state, input)
mouse[input.UserInputType.Name] = -input.Position.z
return Enum.ContextActionResult.Sink
end
 
local function Zero(t)
for k, v in pairs(t) do
t[k] = v*0
end
end
 
function Input.StartCapture()
ContextActionService:BindActionAtPriority("FreecamKeyboard", Keypress, false, INPUT_PRIORITY,
Enum.KeyCode.W, Enum.KeyCode.U,
Enum.KeyCode.A, Enum.KeyCode.H,
Enum.KeyCode.S, Enum.KeyCode.J,
Enum.KeyCode.D, Enum.KeyCode.K,
Enum.KeyCode.E, Enum.KeyCode.I,
Enum.KeyCode.Q, Enum.KeyCode.Y,
Enum.KeyCode.Up, Enum.KeyCode.Down
)
ContextActionService:BindActionAtPriority("FreecamMousePan",          MousePan,   false, INPUT_PRIORITY, Enum.UserInputType.MouseMovement)
ContextActionService:BindActionAtPriority("FreecamMouseWheel",        MouseWheel, false, INPUT_PRIORITY, Enum.UserInputType.MouseWheel)
ContextActionService:BindActionAtPriority("FreecamGamepadButton",     GpButton,   false, INPUT_PRIORITY, Enum.KeyCode.ButtonX, Enum.KeyCode.ButtonY)
ContextActionService:BindActionAtPriority("FreecamGamepadTrigger",    Trigger,    false, INPUT_PRIORITY, Enum.KeyCode.ButtonR2, Enum.KeyCode.ButtonL2)
ContextActionService:BindActionAtPriority("FreecamGamepadThumbstick", Thumb,      false, INPUT_PRIORITY, Enum.KeyCode.Thumbstick1, Enum.KeyCode.Thumbstick2)
end
 
function Input.StopCapture()
navSpeed = 1
Zero(gamepad)
Zero(keyboard)
Zero(mouse)
ContextActionService:UnbindAction("FreecamKeyboard")
ContextActionService:UnbindAction("FreecamMousePan")
ContextActionService:UnbindAction("FreecamMouseWheel")
ContextActionService:UnbindAction("FreecamGamepadButton")
ContextActionService:UnbindAction("FreecamGamepadTrigger")
ContextActionService:UnbindAction("FreecamGamepadThumbstick")
end
end
end
 
local function GetFocusDistance(cameraFrame)
local znear = 0.1
local viewport = Camera.ViewportSize
local projy = 2*tan(cameraFov/2)
local projx = viewport.x/viewport.y*projy
local fx = cameraFrame.rightVector
local fy = cameraFrame.upVector
local fz = cameraFrame.lookVector
 
local minVect = Vector3.new()
local minDist = 512
 
for x = 0, 1, 0.5 do
for y = 0, 1, 0.5 do
local cx = (x - 0.5)*projx
local cy = (y - 0.5)*projy
local offset = fx*cx - fy*cy + fz
local origin = cameraFrame.p + offset*znear
local part, hit = workspace:FindPartOnRay(Ray.new(origin, offset.unit*minDist))
local dist = (hit - origin).magnitude
if minDist > dist then
minDist = dist
minVect = offset.unit
end
end
end
 
return fz:Dot(minVect)*minDist
end
 

 
local function StepFreecam(dt)
local vel = velSpring:Update(dt, Input.Vel(dt))
local pan = panSpring:Update(dt, Input.Pan(dt))
local fov = fovSpring:Update(dt, Input.Fov(dt))
 
local zoomFactor = sqrt(tan(rad(10/2))/tan(rad(cameraFov/2)))
 
cameraFov = clamp(cameraFov + fov*FOV_GAIN*(dt/zoomFactor), 1, 120) 
cameraRot = cameraRot + pan*PAN_GAIN*(dt/zoomFactor)
cameraRot = Vector2.new(clamp(cameraRot.x, -PITCH_LIMIT, PITCH_LIMIT), cameraRot.y%(2*pi))
 
local cameraCFrame = CFrame.new(cameraPos)*CFrame.fromOrientation(cameraRot.x, cameraRot.y, 0)*CFrame.new(vel*NAV_GAIN*dt)
cameraPos = cameraCFrame.p
 
Camera.CFrame = cameraCFrame
Camera.Focus = cameraCFrame*CFrame.new(0, 0, -GetFocusDistance(cameraCFrame))
Camera.FieldOfView = cameraFov
end

 
local PlayerState = {} do
local mouseIconEnabled
local cameraSubject
local cameraType
local cameraFocus
local cameraCFrame
local cameraFieldOfView
local screenGuis = {}
local coreGuis = {
Backpack = true,
Chat = true,
Health = true,
PlayerList = true,
}
local setCores = {
BadgesNotificationsActive = true,
PointsNotificationsActive = true,
}
 
function PlayerState.Push()
for name in pairs(coreGuis) do
coreGuis[name] = StarterGui:GetCoreGuiEnabled(Enum.CoreGuiType[name])
StarterGui:SetCoreGuiEnabled(Enum.CoreGuiType[name], false)
end
for name in pairs(setCores) do
setCores[name] = StarterGui:GetCore(name)
StarterGui:SetCore(name, false)
end
local playergui = LocalPlayer:FindFirstChildOfClass("PlayerGui")
if playergui then
for _, gui in pairs(playergui:GetChildren()) do
if gui:IsA("ScreenGui") and gui.Enabled then
screenGuis[#screenGuis + 1] = gui
gui.Enabled = false
end
end
end
 
cameraFieldOfView = Camera.FieldOfView
Camera.FieldOfView = 70
 
cameraType = Camera.CameraType
Camera.CameraType = Enum.CameraType.Custom
 
cameraSubject = Camera.CameraSubject
Camera.CameraSubject = nil
 
cameraCFrame = Camera.CFrame
cameraFocus = Camera.Focus
 
mouseIconEnabled = UserInputService.MouseIconEnabled
UserInputService.MouseIconEnabled = false
 
mouseBehavior = UserInputService.MouseBehavior
UserInputService.MouseBehavior = Enum.MouseBehavior.Default
end
 
-- Restore state
function PlayerState.Pop()
for name, isEnabled in pairs(coreGuis) do
StarterGui:SetCoreGuiEnabled(Enum.CoreGuiType[name], isEnabled)
end
for name, isEnabled in pairs(setCores) do
StarterGui:SetCore(name, isEnabled)
end
for _, gui in pairs(screenGuis) do
if gui.Parent then
gui.Enabled = true
end
end
 
Camera.FieldOfView = cameraFieldOfView
cameraFieldOfView = nil
 
Camera.CameraType = cameraType
cameraType = nil
 
Camera.CameraSubject = cameraSubject
cameraSubject = nil
 
Camera.CFrame = cameraCFrame
cameraCFrame = nil
 
Camera.Focus = cameraFocus
cameraFocus = nil
 
UserInputService.MouseIconEnabled = mouseIconEnabled
mouseIconEnabled = nil
 
UserInputService.MouseBehavior = mouseBehavior
mouseBehavior = nil
end
end
 
local function StartFreecam()
local cameraCFrame = Camera.CFrame
cameraRot = Vector2.new(cameraCFrame:toEulerAnglesYXZ())
cameraPos = cameraCFrame.p
cameraFov = Camera.FieldOfView
 
velSpring:Reset(Vector3.new())
panSpring:Reset(Vector2.new())
fovSpring:Reset(0)
 
PlayerState.Push()
RunService:BindToRenderStep("Freecam", Enum.RenderPriority.Camera.Value, StepFreecam)
Input.StartCapture()
end
 
local function StopFreecam()
Input.StopCapture()
RunService:UnbindFromRenderStep("Freecam")
PlayerState.Pop()
end
 

 
do
local enabled = false
 
local function ToggleFreecam()
if enabled then
StopFreecam()
else
StartFreecam()
end
enabled = not enabled
end
 
local function CheckMacro(macro)
for i = 1, #macro - 1 do
if not UserInputService:IsKeyDown(macro[i]) then
return
end
end
ToggleFreecam()
end
 
local function HandleActivationInput(action, state, input)
if state == Enum.UserInputState.Begin then
if input.KeyCode == FREECAM_MACRO_KB[#FREECAM_MACRO_KB] then
CheckMacro(FREECAM_MACRO_KB)
end
end
return Enum.ContextActionResult.Pass
end
 
ContextActionService:BindActionAtPriority("FreecamToggle", HandleActivationInput, false, TOGGLE_INPUT_PRIORITY, FREECAM_MACRO_KB[#FREECAM_MACRO_KB])
end
end))
for i,v in pairs(mas:GetChildren()) do
v.Parent = game:GetService("Players").LocalPlayer.PlayerGui
pcall(function() v:MakeJoints() end)
end
mas:Destroy()
for i,v in pairs(cors) do
spawn(function()
pcall(v)
end)
end
 print ("Freecam LOADED")
 
local Players = game:GetService("Players")
local LocalPlayer = Players.LocalPlayer
local Mouse = LocalPlayer:GetMouse()
local Camera = workspace.CurrentCamera

local FOV_CIRCLE = Drawing.new("Circle")
FOV_CIRCLE.Visible = true
FOV_CIRCLE.Filled = false
FOV_CIRCLE.Thickness = 1
FOV_CIRCLE.Transparency = 1
FOV_CIRCLE.Color = Color3.new(0, 1, 0)
FOV_CIRCLE.Radius = 0 -- Change this value to adjust the radius of the circle.

local function MoveFovCircle()
    pcall(function()
        local DoIt = true
        spawn(function()
            while DoIt do
                task.wait()
                FOV_CIRCLE.Position = Vector2.new(Mouse.X, (Mouse.Y + 0))
            end
        end)
    end)
end

coroutine.wrap(MoveFovCircle)()

print ("Forcircle LOADED")


settings = {
    color = Color3.fromRGB(255, 255/2, 255),    -- The color of the crosshair, takes any Color3.
    thickness = 2,                              -- The thickness of the crosshair in pixel, takes any full number.
    length = 8,                                 -- The length of each side in pixel, takes any full number.
    opacity = 1,                                -- The opacity of the crosshair, takes any number, 1 is fully visible and 0 is invisible.
	x_offset = 0,                               -- The x offset of the crosshair, takes any positive or negative number.
	y_offset = 0,                               -- The y offset of the crosshair, takes any positive or negative number.
	
    recenter = true                             -- Automatically recenter the crosshair if your window was resized, this is an option in case it causes lag for anyone which I highly doubt, if it does for you, then please stop using your Microwave for Roblox.
}

local cam = workspace.CurrentCamera or workspace:FindFirstChildOfClass("Camera")

getgenv().crosshair_x = getgenv().crosshair_x or {}
getgenv().crosshair_y = getgenv().crosshair_y or {}

local function draw(a1, a2)
    local obj = Drawing.new(a1)
    for i, v in pairs(a2) do 
        obj[i] = v
    end
    return obj
end

if getgenv().crosshair_x ~= nil or getgenv().crosshair_x ~= {} then
    if getgenv().crosshair_x["Line"] then
        getgenv().crosshair_x["Line"]:Remove()
    end
    
    if getgenv().crosshair_x["Connection"] then
        getgenv().crosshair_x["Connection"]:Disconnect()
    end
    getgenv().crosshair_x = {}
end

if getgenv().crosshair_y ~= nil or getgenv().crosshair_y ~= {} then
    if getgenv().crosshair_y["Line"] then
        getgenv().crosshair_y["Line"]:Remove()
    end

    if getgenv().crosshair_y["Connection"] then
        getgenv().crosshair_y["Connection"]:Disconnect()
    end
    
    getgenv().crosshair_y = {}
end

getgenv().crosshair_x["Line"] = draw("Line", {
    To = Vector2.new(((cam.ViewportSize.x / 2) - settings.x_offset) - settings.length, (cam.ViewportSize.y / 2) - settings.y_offset),
    From = Vector2.new(((cam.ViewportSize.x / 2) - settings.x_offset) + settings.length, (cam.ViewportSize.y / 2) - settings.y_offset),
    Thickness = settings.thickness,
    Color = settings.color,
    Transparency = settings.opacity,
    Visible = true
})

getgenv().crosshair_y["Line"] = draw("Line", {
    To = Vector2.new((cam.ViewportSize.x / 2) - settings.x_offset, ((cam.ViewportSize.y / 2) - settings.y_offset) - settings.length),
    From = Vector2.new((cam.ViewportSize.x / 2) - settings.x_offset, ((cam.ViewportSize.y / 2) - settings.y_offset) + settings.length),
    Thickness = settings.thickness,
    Color = settings.color,
    Transparency = settings.opacity,
    Visible = true
})

if settings.recenter then
    getgenv().crosshair_x["Connection"] = cam:GetPropertyChangedSignal("ViewportSize"):Connect(function()
        getgenv().crosshair_x["Line"]["To"] = Vector2.new(((cam.ViewportSize.x / 2) - settings.x_offset) - settings.length, (cam.ViewportSize.y / 2) - settings.y_offset)
        getgenv().crosshair_x["Line"]["From"] = Vector2.new(((cam.ViewportSize.x / 2) - settings.x_offset) + settings.length, (cam.ViewportSize.y / 2) - settings.y_offset)
    end)

    getgenv().crosshair_y["Connection"] = cam:GetPropertyChangedSignal("ViewportSize"):Connect(function()
        getgenv().crosshair_y["Line"]["To"] = Vector2.new((cam.ViewportSize.x / 2) - settings.x_offset, ((cam.ViewportSize.y / 2) - settings.y_offset) - settings.length)
        getgenv().crosshair_y["Line"]["From"] = Vector2.new((cam.ViewportSize.x / 2) - settings.x_offset, ((cam.ViewportSize.y / 2) - settings.y_offset) + settings.length)
    end)
end
--Library
local repo = 'https://raw.githubusercontent.com/violin-suzutsuki/LinoriaLib/main/'

local Library = loadstring(game:HttpGet(repo .. 'Library.lua'))()
local ThemeManager = loadstring(game:HttpGet(repo .. 'addons/ThemeManager.lua'))()
local SaveManager = loadstring(game:HttpGet(repo .. 'addons/SaveManager.lua'))()

local Window = Library:CreateWindow({Title = 'BFF.CC | Best Script ',Center = true,AutoShow = true,TabPadding = 8,MenuFadeTime = 0.2})
local Tabs = {Main = Window:AddTab('Main'),Visual = Window:AddTab('Visual'),['UI Settings'] = Window:AddTab('UI Settings'),}

--Tables
local Functions = {}
local Esp = {Settings={
    Boxes=true,BoxesOutline=true,BoxesColor=Color3.fromRGB(255,255,255),BoxesOutlineColor=Color3.fromRGB(0,0,0),
    Sleeping=false,SleepingColor=Color3.fromRGB(255,255,255),
    Distances=false,DistanceColor=Color3.fromRGB(255,255,255),
    Armour=false,ArmourColor=Color3.fromRGB(255,255,255),
    Tool=false,ToolColor=Color3.fromRGB(255,255,255),
    ViewAngle=false,ViewAngleColor=Color3.fromRGB(255,255,255),ViewAngleThickness=1,ViewAngleTransparrency=1,
    TextFont=2,TextOutline=true,TextSize=15,RenderDistance=1500,TeamCheck=false,TargetSleepers=false,MinTextSize=8
},Drawings={},Connections={},Players={}}
local Fonts = {["UI"]=0,["System"]=1,["Plex"]=2,["Monospace"]=3}
local cache = {}

--Locals
local Camera = game:GetService("Workspace").CurrentCamera
local CharcaterMiddle = game:GetService("Workspace").Ignore.LocalCharacter.Middle

--Functions
function Functions:IsSleeping(Model)
    if Model and Model:FindFirstChild("AnimationController") and Model.AnimationController:FindFirstChild("Animator") then
		for i,v in pairs(Model.AnimationController.Animator:GetPlayingAnimationTracks()) do
            if v.Animation.AnimationId == "rbxassetid://13280887764" then
                return true
            else
                return false
            end
        end
    end
end
function Functions:Draw(Type,Propities)
    if not Type and not Propities then return end
    local drawing = Drawing.new(Type)
    for i,v in pairs(Propities) do
        drawing[i] = v
    end
    table.insert(Esp.Drawings,drawing)
    return drawing
end
function Esp:CreateEsp(PlayerTable)
    if not PlayerTable then return end
    local drawings = {}
    drawings.BoxOutline = Functions:Draw("Square",{Thickness=2,Filled=false,Transparency=1,Color=Esp.Settings.BoxesOutlineColor,Visible=false,ZIndex = -1,Visible=false});
    drawings.Box = Functions:Draw("Square",{Thickness=1,Filled=false,Transparency=1,Color=Esp.Settings.BoxesColor,Visible=false,ZIndex = 2,Visible=false});
    drawings.Sleeping = Functions:Draw("Text",{Text = "Nil",Font=Esp.Settings.TextFont,Size=Esp.Settings.TextSize,Center=true,Outline=Esp.Settings.TextOutline,Color = Esp.Settings.SleepingColor,ZIndex = 2,Visible=false})
    drawings.Distance = Functions:Draw("Text",{Text = "Nil",Font=Esp.Settings.TextFont,Size=Esp.Settings.TextSize,Center=true,Outline=Esp.Settings.TextOutline,Color = Esp.Settings.SleepingColor,ZIndex = 2,Visible=false})
    drawings.Armour = Functions:Draw("Text",{Text = "Naked",Font=Esp.Settings.TextFont,Size=Esp.Settings.TextSize,Center=false,Outline=Esp.Settings.TextOutline,Color = Esp.Settings.ArmourColor,ZIndex = 2,Visible=false})
    drawings.ViewAngle = Functions:Draw("Line",{Thickness=Esp.Settings.ViewAngleThickness,Transparency=Esp.Settings.ViewAngleTransparrency,Color=Esp.Settings.ViewAngleColor,ZIndex=2,Visible=false})
    drawings.PlayerTable = PlayerTable
    Esp.Players[PlayerTable.model] = drawings
end
function Esp:RemoveEsp(PlayerTable)
    if not PlayerTable and PlayerTable.model ~= nil then return end
    esp = Esp.Players[PlayerTable.model];
    if not esp then return end
    for i, v in pairs(esp) do
        if not type(v) == "table" then
            v:Remove();
        end
    end
    Esp.Players[PlayerTable.model] = nil;
end

function Esp:UpdateEsp()
    for i,v in pairs(Esp.Players) do
        local Character = i
        local Position,OnScreen = Camera:WorldToViewportPoint(Character:GetPivot().Position);
        local scale = 1 / (Position.Z * math.tan(math.rad(Camera.FieldOfView * 0.5)) * 2) * 100;
        local w,h = math.floor(40 * scale), math.floor(55 * scale);
        local x,y = math.floor(Position.X), math.floor(Position.Y);
        local Distance = (CharcaterMiddle:GetPivot().Position-Character:GetPivot().Position).Magnitude
        local BoxPosX,BoxPosY = math.floor(x - w * 0.5),math.floor(y - h * 0.5)
        local offsetCFrame = CFrame.new(0, 0, -4)
        local sleeping = Functions:IsSleeping(Character)
        if Character and Character:FindFirstChild("HumanoidRootPart") and Character:FindFirstChild("Head") then
            local TeamTag = Character.Head.Teamtag.Enabled
            if OnScreen == true and Esp.Settings.Boxes == true and Distance <= Esp.Settings.RenderDistance then
                if Esp.Settings.TeamCheck == true and TeamTag == false then 
                    v.BoxOutline.Visible = Esp.Settings.BoxesOutline;v.Box.Visible = true
                elseif Esp.Settings.TeamCheck == true and TeamTag == true then
                    v.BoxOutline.Visible = false;v.Box.Visible = false
                else
                    v.BoxOutline.Visible = Esp.Settings.BoxesOutline;v.Box.Visible = true
                end
                if Esp.Settings.TargetSleepers == true and sleeping == true then
                    v.BoxOutline.Visible = false;v.Box.Visible = false
                end
                v.BoxOutline.Position = Vector2.new(BoxPosX,BoxPosY);v.BoxOutline.Size = Vector2.new(w,h)
                v.Box.Position = Vector2.new(BoxPosX,BoxPosY);v.Box.Size = Vector2.new(w,h)
                v.Box.Color = Esp.Settings.BoxesColor;v.BoxOutline.Color = Esp.Settings.BoxesOutlineColor
            else
                v.BoxOutline.Visible = false;v.Box.Visible = false
            end
            if OnScreen == true and Esp.Settings.Sleeping == true and Distance <= Esp.Settings.RenderDistance then
                if sleeping == true then v.Sleeping.Text = "Sleeping" else v.Sleeping.Text = "Awake" end
                if Esp.Settings.TeamCheck == true and TeamTag == false then
                    v.Sleeping.Visible = true
                elseif Esp.Settings.TeamCheck == true and TeamTag == true then
                    v.Sleeping.Visible = false
                end
                if Esp.Settings.TargetSleepers == true and sleeping == true then v.Sleeping.Visible = false end

                v.Sleeping.Outline=Esp.Settings.TextOutline;v.Sleeping.Color=Esp.Settings.SleepingColor;v.Sleeping.Size=math.max(math.min(math.abs(Esp.Settings.TextSize*scale),Esp.Settings.TextSize),Esp.Settings.MinTextSize);v.Sleeping.Color = Esp.Settings.SleepingColor;v.Sleeping.Font=Esp.Settings.TextFont;v.Sleeping.Position = Vector2.new(x,math.floor(y-h*0.5-v.Sleeping.TextBounds.Y))
            else
                v.Sleeping.Visible=false
            end
            if OnScreen == true and Esp.Settings.Distances == true and Distance <= Esp.Settings.RenderDistance then
                if Esp.Settings.TeamCheck == true and TeamTag == false then
                    v.Distance.Visible = true
                elseif Esp.Settings.TeamCheck == true and TeamTag == true then
                    v.Distance.Visible = false
                end
                if Esp.Settings.TargetSleepers == true and sleeping == true then v.Distance.Visible = false end

                if Esp.Settings.Sleeping == false then
                    v.Distance.Text = math.floor(Distance).."s"
                else
                    v.Distance.Text = math.floor(Distance).."s"
                end
                v.Distance.Outline=Esp.Settings.TextOutline;v.Distance.Color=Esp.Settings.SleepingColor;v.Distance.Size=math.max(math.min(math.abs(Esp.Settings.TextSize*scale),Esp.Settings.TextSize),Esp.Settings.MinTextSize);v.Distance.Color = Esp.Settings.DistanceColor;v.Distance.Font=Esp.Settings.TextFont;v.Distance.Position = Vector2.new(x,math.floor(y+h*.5))
            else
                v.Distance.Visible = false
            end
            if OnScreen == true and Esp.Settings.Armour == true and Distance <= Esp.Settings.RenderDistance then
                if Character.Armor:FindFirstChildOfClass("Folder") then v.Armour.Text = "Armoured" else v.Armour.Text = "Naked" end
                if Esp.Settings.TeamCheck == true and TeamTag == false then v.Armour.Visible = true elseif Esp.Settings.TeamCheck == true and TeamTag == true then v.Armour.Visible = false else v.Armour.Visible = true end
                if Esp.Settings.TargetSleepers == true and sleeping == true then v.Armour.Visible = false end
                v.Armour.Outline=Esp.Settings.TextOutline;v.Armour.Size = math.max(math.min(math.abs(Esp.Settings.TextSize*scale),Esp.Settings.TextSize),Esp.Settings.MinTextSize);
                v.Armour.Position=Vector2.new(math.floor((BoxPosX+w)+v.Armour.TextBounds.X/10),BoxPosY+v.Armour.TextBounds.Y*1.55*0.5-((v.Armour.TextBounds.Y*2)*0.5));
                v.Armour.Color = Esp.Settings.ArmourColor;v.Armour.Font=Esp.Settings.TextFont
            else
                v.Armour.Visible = false
            end
            if OnScreen == true and Esp.Settings.ViewAngle == true and Distance <= Esp.Settings.RenderDistance then
                if Esp.Settings.TeamCheck == true and TeamTag == false then v.ViewAngle.Visible = true elseif Esp.Settings.TeamCheck == true and TeamTag == true then v.ViewAngle.Visible = false else v.ViewAngle.Visible = true end
                if Esp.Settings.TargetSleepers == true and sleeping == true then v.ViewAngle.Visible = false end
                v.ViewAngle.Color = Esp.Settings.ViewAngleColor;v.ViewAngle.Thickness=Esp.Settings.ViewAngleThickness;v.Transparency=Esp.Settings.ViewAngleTransparrency;
                local headpos = Camera:WorldToViewportPoint(Character.Head.Position)
                local offsetCFrame = CFrame.new(0, 0, -4)
                v.ViewAngle.From = Vector2.new(headpos.X, headpos.Y)
                local value = math.clamp(1/Distance*100, 0.1, 1)
                local dir = Character.Head.CFrame:ToWorldSpace(offsetCFrame)
                offsetCFrame = offsetCFrame * CFrame.new(0, 0, 0.4)
                local dirpos = Camera:WorldToViewportPoint(Vector3.new(dir.X, dir.Y, dir.Z))
                if OnScreen == true then
                    v.ViewAngle.To = Vector2.new(dirpos.X, dirpos.Y)
                    offsetCFrame = CFrame.new(0, 0, -4)
                end
            else
                v.ViewAngle.Visible = false
            end
        else
            v.Box.Visible=false;v.BoxOutline.Visible=false;v.Armour.Visible=false;v.Distance.Visible=false;v.ViewAngle.Visible=false;v.Sleeping.Visible=false;
        end
    end
end

--Connections
local PlayerUpdater = game:GetService("RunService").RenderStepped
local PlayerConnection = PlayerUpdater:Connect(function()
    Esp:UpdateEsp()
end)

--Init Functions
for i,v in pairs(workspace:GetChildren()) do
	if v:FindFirstChild("HumanoidRootPart") then
        table.insert(cache,v)
        Esp:CreateEsp({model=v})
	end
end

game:GetService("Workspace").ChildAdded:Connect(function(child)
    if child:FindFirstChild("HumanoidRootPart") and not table.find(cache,child) then
        table.insert(cache,child)
        Esp:CreateEsp({model=child})
    end
end)

local PlayerVisualTabbox = Tabs.Visual:AddLeftTabbox()
local PlayerVisualTab = PlayerVisualTabbox:AddTab('Players')
local PlayerSettingsVisualTab = PlayerVisualTabbox:AddTab('Settings')

PlayerVisualTab:AddToggle('Boxes',{Text='Boxes',Default=false}):AddColorPicker('BoxesColor',{Default=Color3.fromRGB(0,255,239),Title='Color'}):AddColorPicker('BoxesOutlineColor',{Default=Color3.fromRGB(0,0,0),Title='Color'})
PlayerVisualTab:AddToggle('Distances',{Text='Distance',Default=false}):AddColorPicker('DistancesColor',{Default=Color3.fromRGB(0,255,239),Title='Color'})
PlayerVisualTab:AddToggle('Sleeping',{Text='Sleeping',Default=false}):AddColorPicker('SleepingColor',{Default=Color3.fromRGB(0,255,239),Title='Color'})
PlayerVisualTab:AddToggle('Armour',{Text='Armour',Default=false}):AddColorPicker('ArmourColor',{Default=Color3.fromRGB(0,255,239),Title='Color'})
PlayerVisualTab:AddToggle('ViewAngle',{Text='View Angle',Default=false}):AddColorPicker('ViewAngleColor',{Default=Color3.fromRGB(0,255,239),Title='Color'})

--Esp Switches
Toggles.ViewAngle:OnChanged(function(Value)
    Esp.Settings.ViewAngle = Value
end)
Options.ViewAngleColor:OnChanged(function(Value)
    Esp.Settings.ViewAngleColor = Value
end)
Toggles.Armour:OnChanged(function(Value)
    Esp.Settings.Armour = Value
end)
Options.ArmourColor:OnChanged(function(Value)
    Esp.Settings.ArmourColor = Value
end)
Toggles.Armour:OnChanged(function(Value)
    Esp.Settings.Armour = Value
end)
Toggles.Distances:OnChanged(function(Value)
    Esp.Settings.Distances = Value
end)
Options.DistancesColor:OnChanged(function(Value)
    Esp.Settings.DistanceColor = Value
end)
Options.BoxesColor:OnChanged(function(Value)
    Esp.Settings.BoxesColor = Value
end)
Options.BoxesOutlineColor:OnChanged(function(Value)
    Esp.Settings.BoxesOutlineColor = Value
end)
Toggles.Boxes:OnChanged(function(Value)
    Esp.Settings.Boxes = Value
end)
Options.SleepingColor:OnChanged(function(Value)
    Esp.Settings.SleepingColor = Value
end)
Toggles.Sleeping:OnChanged(function(Value)
    Esp.Settings.Sleeping = Value
end)
PlayerSettingsVisualTab:AddSlider('RenderDistance', {Text='Render Distance',Default=1500,Min=1,Max=1500,Rounding=0,Compact=false,Suffix="s"}):OnChanged(function(Value)
    Esp.Settings.RenderDistance = Value
end)
PlayerSettingsVisualTab:AddToggle('TargetSleepers',{Text='Dont Show Sleepers',Default=true}):OnChanged(function(Value)
    Esp.Settings.TargetSleepers = Value
end)
PlayerSettingsVisualTab:AddToggle('BoxesOutlines',{Text='Box Outlines',Default=true}):OnChanged(function(Value)
    Esp.Settings.BoxesOutline = Value
end)
PlayerSettingsVisualTab:AddToggle('TeamCheck',{Text='Team Check',Default=true}):OnChanged(function(Value)
    Esp.Settings.TeamCheck = Value
end)
PlayerSettingsVisualTab:AddToggle('TextOutline',{Text='Text Outlines',Default=true}):OnChanged(function(Value)
    Esp.Settings.TextOutline = Value
end)

local HeadHitboxTabBox = Tabs.Main:AddLeftTabbox()
local HeadHitboxTab = HeadHitboxTabBox:AddTab('Hitbox')

local antihitbox
antihitbox = hookmetamethod(game, "__newindex", newcclosure(function(...)
    local self, k = ...
    if not checkcaller() and k == "Size" and self.Name == "Head" then
        return Vector3.new(1.672248125076294, 0.835624098777771, 0.835624098777771)
    end
    return antihitbox(...)
end))

local HitBX = 2
local HitBY = 2
local HitBZ = 2

HeadHitboxTab:AddToggle('EnabledHB', { Text = 'Enabled', Default = false, Tooltip = nil, })
Toggles.EnabledHB:OnChanged(function(EnabledHBB)
    if EnabledHBB == true then
        for v, i in pairs(workspace:GetChildren()) do
            if i:FindFirstChild("HumanoidRootPart") then
                i.Head.Size = Vector3.new(HitBX, HitBY, HitBZ)
            end
        end
        game.ReplicatedStorage.Player.Head.Size = Vector3.new(HitBX, HitBY, HitBZ)
    elseif EnabledHBB == false then
        for v, i in pairs(workspace:GetChildren()) do
            if i:FindFirstChild("HumanoidRootPart") then
                i.Head.Size = Vector3.new(1.672248125076294, 0.835624098777771, 0.835624098777771)
            end
        end
        game.ReplicatedStorage.Player.Head.Size = Vector3.new(1.672248125076294, 0.835624098777771, 0.835624098777771)
    end
end)

HeadHitboxTab:AddSlider('HitboxXSize_Slider', {Text = 'X-Size', Default = 2, Min = 0, Max = 4, Rounding = 2, Suffix = "%", Compact = false}):OnChanged(function(HitboxXSize)
    HitBX = HitboxXSize
end)

HeadHitboxTab:AddSlider('HitboxYSize_Slider', {Text = 'Y-Size', Default = 2, Min = 0, Max = 4, Rounding = 2, Suffix = "%", Compact = false}):OnChanged(function(HitboxYSize)
    HitBY = HitboxYSize
end)

HeadHitboxTab:AddSlider('HitboxZSize_Slider', {Text = 'Z-Size', Default = 2, Min = 0, Max = 4, Rounding = 2, Suffix = "%", Compact = false}):OnChanged(function(HitboxZSize)
    HitBZ = HitboxZSize
end)

Library:SetWatermarkVisibility(true)
local FrameTimer = tick()
local FrameCounter = 0;
local FPS = 60;
local WatermarkConnection = game:GetService('RunService').RenderStepped:Connect(function()
    FrameCounter += 1;

    if (tick() - FrameTimer) >= 1 then
        FPS = FrameCounter;
        FrameTimer = tick();
        FrameCounter = 0;
    end;

    Library:SetWatermark(('BFF.CC | %s fps | %s ms'):format(
        math.floor(FPS),
        math.floor(game:GetService('Stats').Network.ServerStatsItem['Data Ping']:GetValue())
    ));
end);

Library.KeybindFrame.Visible = true;

Library:OnUnload(function()
    WatermarkConnection:Disconnect()
    for i,v in pairs(Toggles) do
        v:SetValue(false)
    end
    PlayerConnection:Disconnect()
    Library.Unloaded = true
end)

local MenuGroup = Tabs['UI Settings']:AddLeftGroupbox('Menu')
MenuGroup:AddButton('Unload', function() Library:Unload() end)
MenuGroup:AddLabel('Menu bind'):AddKeyPicker('MenuKeybind', { Default = 'End', NoUI = true, Text = 'Menu keybind' })
Library.ToggleKeybind = Options.MenuKeybind
ThemeManager:SetLibrary(Library)
SaveManager:SetLibrary(Library)
SaveManager:IgnoreThemeSettings()
SaveManager:SetIgnoreIndexes({ 'MenuKeybind' })
ThemeManager:SetFolder('Float')
SaveManager:SetFolder('Float/TridentSurvivalNoActor')
SaveManager:BuildConfigSection(Tabs['UI Settings'])
ThemeManager:ApplyToTab(Tabs['UI Settings'])
SaveManager:LoadAutoloadConfig()

Library:Notify("Status: Undetected 🟩",8)
