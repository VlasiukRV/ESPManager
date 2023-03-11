;
(function (exp) {
    if (!exp.ESP) {
        exp.ESP = new Object(null);
    }
    var ESP = exp.ESP;

    addActionUpdateSettings = function () {

        $(".update_settings").on("click", (event)=>{
            event.preventDefault();

            var formValues= $(event.target).parent().serialize();

            $.post("/updateSettings", formValues, function(data){
                ESP.loadingSettings();
            });
        });

        $(".add-led-sensor").on("click", (event)=>{
            event.preventDefault();

            var formValues= $(event.target).parent().serialize();

            $.post("/addLedSensor", formValues, function(data){
                ESP.loadingSettings();
            });
        });
    }

    ESP.loadingSettings = function () {
        const $espSettings = $('#esp-settings');
        const $espSensors = $('#esp-sensors')
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

/*
                const esp_properties_html = '';
                $.each(data.espProperties, function(key, value){

                });
                $espSettings.html(esp_properties_html);
*/

                const esp_settings_html = data.settings
                    .map(item => `<li class="list-group-item">${item.synonym}: <form> <input type="text" name="${item.name}" value="${item.value}"> <input type="submit" value="Submit" class="update_settings"> </form> </li>`)
                    .join('');

                $espSettings.html(esp_settings_html);

                const esp_sensors_html = data.sensors
                    .map(item => `
                    <div class="card border-secondary mb-3">
                        <h5 id="${item.name}" class="card-header">${item.synonym}</h5>
                        <div class="card-body">
                            <h6 class="card-subtitle mb-2 text-muted">GPIO state: <strong> %GPIO_STATE%</strong></h6>
                            <div class="card-text">
                                
                            </div>
                        </div>
                        <div class="card-footer">
                            <a href="#" class="card-link">Settings</a>
                        </div>
                    </div>`)
                    .join('');
                $espSensors.html(esp_sensors_html);

                addActionUpdateSettings();

                $raw.text(JSON.stringify(data, undefined, 2));
            }
        });

    }

})(window);
$(document).ready(() => {

    ESP.loadingSettings();

    $('#get-settings').on('click', (event) => {
        event.preventDefault();

        ESP.loadingSettings();

    });

});