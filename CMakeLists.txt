idf_component_register(SRCS "water_can.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES "elrebo-de__shelly_plug" "elrebo-de__hcsr04_sensor"
                             "stream" "sachin42__httpclient")

