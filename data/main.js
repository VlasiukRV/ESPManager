;
(function (exp) {
    if (!exp.ESP) {
        exp.ESP = new Object(null);
    }
    let ESP = exp.ESP;

    // Create a client instance
    var MQTTclient = new Paho.MQTT.Client('appesp32.uptarget.co', 1082, "clientId");
    // connect the client
    MQTTclient.connect({
        userName: 'admin',
        password: '123456',
        onSuccess: function () {
            console.log("onConnect");

            ESP.loadingSettings();
            ESP.loadingConfiguration();
            ESP.addAction();

        }
    });
    // set callback handlers
    MQTTclient.onConnectionLost = function (responseObject) {

        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }

    };
    MQTTclient.onMessageArrived = function (message) {

        let div_id = "#" + message.destinationName.replace(/\//g, "-");
        $(div_id).text(message.payloadString);

    };

    ESP.sendMessageMQTTBroker = function (mqtt_topic, message) {

        console.log("Send message to mgtt brocker");
        MQTT_message = new Paho.MQTT.Message(message);
        MQTT_message.destinationName = mqtt_topic;
        MQTTclient.send(MQTT_message);

    }

    ESP.addAction = function () {

        $('#get-settings').on('click', (event) => {
            event.preventDefault();

            ESP.loadingSettings();

        });

        $('#get-configuration').on('click', (event) => {
            event.preventDefault();

            ESP.loadingConfiguration();

        });

        $(".update_settings").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/updateSettings", formValues, function(data){
                ESP.loadingSettings();
            });
        });

        $(".update_configuration").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/updateConfiguration", formValues, function(data){
                ESP.loadingConfiguration();
            });
        });

        $("#select-sensor-type").on("change", function() {

            let sensorType = this.value.toLowerCase();
            $('[name="sensor-type"]').hide();
            $('#sensor-type-' + sensorType).show();

        });

        $(".add-led-sensor").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/addSensor", formValues, function(data){
                $('[name="sensor-type"]').hide();
                ESP.loadingConfiguration();
            });
        });

        $(".add-dht-sensor").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/addSensor", formValues, function(data){
                $('[name="sensor-type"]').hide();
                ESP.loadingConfiguration();
            });
        });

        $(".add-motion-sensor").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/addSensor", formValues, function(data){
                $('[name="sensor-type"]').hide();
                ESP.loadingConfiguration();
            });
        });

    }

    ESP.loadingSettings = function () {
        const $espSettings = $('#esp-settings');
        const $raw = $('pre');

        $espSettings.text('Loading the JSON file.');

        $.ajax({
            type: 'GET',
            url: '/settings',
            async: false,
            beforeSend: (xhr) => {
                if (xhr && xhr.overrideMimeType) {
                    xhr.overrideMimeType('application/json;charset=utf-8');
                }
            },
            dataType: 'json',
            success: (data) => {

                let esp_settings_html = data.settings
                    .map(item => `<li class="list-group-item"> <form class="form-group"> <label>${item.synonym}:</label> <input class="form-control" type="text" name="${item.name}" value="${item.value}"> <button type="submit" value="Submit" class="btn btn-primary update_settings"> Submit </button> </form> </li>`)
                    .join('');

                $espSettings.html(esp_settings_html);

                //$raw.text(JSON.stringify(data, undefined, 2));
            }
        });

    }

    const get_censor_card_html = (item, item_keys_html, item_action_html) => `
    <div class="card border-secondary mb-3">
        <h5 id="${item.name}" class="card-header">${item.synonym}</h5>
        <div class="card-body">
            <h6 class="card-subtitle mb-2 text-muted">GPIO state: <strong> %GPIO_STATE%</strong></h6>
            <div class="card-text">
                ${item_keys_html}
            </div>
        </div>
        <div class="card-footer">
            ${item_action_html}
        </div>
    </div>`;

    ESP.loadingConfiguration = function () {
        const $espSensors = $('#esp-sensors');

        $.ajax({
            type: 'GET',
            url: '/configuration',
            async: false,
            beforeSend: (xhr) => {
                if (xhr && xhr.overrideMimeType) {
                    xhr.overrideMimeType('application/json;charset=utf-8');
                }
            },
            dataType: 'json',
            success: (data) => {

                let esp_sensors_html = '';

                esp_sensors_html = esp_sensors_html + data.sensors.filter(item=>{
                    return (item.type === 'led');
                })
                    .map(item => {
                        let mqtt_topic = item.properties.mqtt_topic;
                        MQTTclient.subscribe(""+mqtt_topic+"");

                        let item_action_html = `
                                <button onclick="ESP.sendMessageMQTTBroker('${item.properties.mqtt_topic}', 'on')" class="card-link">On</button>
                                <button onclick="ESP.sendMessageMQTTBroker('${item.properties.mqtt_topic}', 'off')" class="card-link">Off</button>
                                <button onclick="ESP.sendMessageMQTTBroker('${item.properties.mqtt_topic}', 'blink')" class="card-link">Blink</button>`;

                        let item_keys_html = Object.keys(item.value).map((key, i) => {

                            let key_id = ""+mqtt_topic.replace(/\//g, "-");
                            return `
                            <p>
                                <span>${key}: <div id="${key_id}">${item.value[key]}</div></span>
                            </p>`;
                    }).join('');

                        return get_censor_card_html(item, item_keys_html, item_action_html) })
                    .join('');

                esp_sensors_html = esp_sensors_html + data.sensors.filter(item=>{
                    return ( item.type !== 'led' );
                })
                    .map(item => {
                        let mqtt_topic = item.properties.mqtt_topic;

                        MQTTclient.subscribe(""+mqtt_topic+"/+");

                        let item_action_html = '';

                        let item_keys_html = Object.keys(item.value).map((key, i) => {
                            let key_id = mqtt_topic.replace(/_/g, "-")+"-"+key;
                            return `
                            <p>
                                <span>${key}: <div id="${key_id}">${item.value[key]}</div> </span>
                            </p>`;
                        }).join('');

                        return get_censor_card_html(item, item_keys_html, item_action_html) })
                    .join('');

                $espSensors.html(esp_sensors_html);

                //$raw.text(JSON.stringify(data, undefined, 2));
            }
        });

    }

})(window);
$(document).ready(() => {

    // ESP.loadingSettings();
    // ESP.loadingConfiguration();
    // ESP.addAction();

});
