;
(function (exp) {
    if (!exp.ESP) {
        exp.ESP = new Object(null);
    }
    let ESP = exp.ESP;

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

            $.post("/addLedSensor", formValues, function(data){
                $('[name="sensor-type"]').hide();
                ESP.loadingConfiguration();
            });
        });

        $(".add-dht-sensor").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/addDHTSensor", formValues, function(data){
                $('[name="sensor-type"]').hide();
                ESP.loadingConfiguration();
            });
        });

        $(".add-motion-sensor").on("click", (event)=>{
            event.preventDefault();

            let formValues= $(event.target).parent().serialize();

            $.post("/addMotionSensor", formValues, function(data){
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

                let esp_sensors_html = data.sensors
                    .map(item => {

                        let keys_html = Object.keys(item.value).map((key, i) => (`
                            <p>
                                <span>${key}: ${item.value[key]}</span>
                            </p>`
                        )).join('');

                        return `
                    <div class="card border-secondary mb-3">
                        <h5 id="${item.name}" class="card-header">${item.synonym}</h5>
                        <div class="card-body">
                            <h6 class="card-subtitle mb-2 text-muted">GPIO state: <strong> %GPIO_STATE%</strong></h6>
                            <div class="card-text">
                                ${keys_html}
                            </div>
                        </div>
                        <div class="card-footer">
                            <a href="#" class="card-link">Settings</a>
                        </div>
                    </div>`})
                    .join('');
                $espSensors.html(esp_sensors_html);

                //$raw.text(JSON.stringify(data, undefined, 2));
            }
        });

    }

})(window);
$(document).ready(() => {

    ESP.loadingSettings();
    ESP.loadingConfiguration();
    ESP.addAction();

});