
import asyncio
import websockets
import json
 
async def test():
    while True:
        async with websockets.connect('wss://2t6ne6hjea.execute-api.us-east-1.amazonaws.com/production') as websocket:
            x=input("Enter message: ")
            l = {"action":"sendmessage","message":f"{x}"}
            await websocket.send(json.dumps(l))
            response = await websocket.recv()
            print(response)
 
asyncio.get_event_loop().run_until_complete(test())