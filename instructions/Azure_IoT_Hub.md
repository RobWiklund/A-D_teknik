## Interkommunikation mellan dessa olika delar



## Data och enheter

Kom ihåg att man inte kan använda samma enhetsnamn och enhetsnyckel för att "logga in" på ett annat ställe, utan till detta används inbyggda endpoints som standard.
[Se IoT-arkitekturen](../README.md#iot-arkitekturen)

## kommunikation

### Endpoints

Dessa slutpunkter kan fås från Azure-portalen. Enpoint får data från det som publicerats från enheten. För att få data prenumererar man på endpoint via API.
Node-RED läser in dessa endpoints med eventhub-receiver: [flows.nodered.org](https://flows.nodered.org/node/node-red-contrib-azure-eventhub-receive)


Formatet för de inbyggda slutpunkterna i Azure.
``` api
Endpoint=sb://[Host]/;SharedAccessKeyName=iothubowner;SharedAccessKey=[Secret key]=;EntityPath=[iothub-path]
```

### Kommunikationssätt

Som standard derigeras meddelanden till den inbyggda tjänstriktade slutpunkten (meddelanden/händelser) som är kompatibel med Event Hubs. Den här slutpunkten exponeras för närvarande endast med hjälp av AMQP-protokollet på port 5671 och AMQP över WebSockets på port 443. Källa: [Microsoft.com](https://learn.microsoft.com/en-us/azure/iot-hub/iot-hub-devguide-messages-read-builtin)

Detta betyder att meddelanden i första hand sänds över TCP men har möjlighet att retirera till WebSockets ifall kommunikation inte upprätts. Detta är också en bidragande orsak varför WebSockets behövs vidarebefordras till lämplig slutpunkt: [NGINX instruktioner](NGINX.md#Auth). När AMQP över WebSockets används för att kommunicera med Azure IoT Hub är det viktigt att se till att WebSocket-anslutningen är korrekt etablerad och konfigurerad för att fungera.
