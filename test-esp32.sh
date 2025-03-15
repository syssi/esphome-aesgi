#!/bin/bash

if [[ $2 == tests/* ]]; then
  C="../components"
else
  C="components"
fi

esphome -s external_components_source components ${1:-run} ${2:-esp32-example.yaml}
