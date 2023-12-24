print("Loads Test Lua")

local thing = Thing()
local creature = Creature()
local player = Player()

local knight = Vocation()
local sorc = Vocation()
local druid = Vocation()
local pally = Vocation()

knight:setId(1)
knight:setName("Knight")

sorc:setId(2)
sorc:setName("Sorceror")

druid:setId(3)
druid:setName("Druid")

pally:setId(4)
pally:setName("Paladin")

print(knight:getName())
print(knight:getId())

knight:register()
sorc:register()
druid:register()
pally:register()

print(thing)

print(creature:getCreature())
print(player:getHealthPoints())
print(player:getPlayer())