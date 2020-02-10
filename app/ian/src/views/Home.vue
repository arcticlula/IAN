<template>
  <div class="home">
    <!-- <img alt="Vue logo" src="../assets/logo.png" /> -->
    <!-- <HelloWorld msg="Welcome to Your Vue.js App" /> -->
    <!-- <ul id="example-1">
      <li v-for="dev in bluetooth.devices.saved" v-bind:key="dev.address">{{ dev.name }}</li>
    </ul>-->
    <!-- <br /> -->
    <!-- <ul id="example-2">
      <li v-for="dev in bluetooth.devices.unpaired" v-bind:key="dev.address">{{ dev.name }}</li>
    </ul>-->
    <!-- {{bluetooth}} -->
    <!-- {{unpaired}} -->
    <b-card-group deck>
      <h5>
        <b-badge>
          {{bluetooth.state}}
          <b-spinner v-if="spinnerShow" small></b-spinner>
        </b-badge>
      </h5>
      <b-card no-body header-tag="header">
        <template v-slot:header>
          <h6 class="mb-0" v-b-toggle.collapse-grav>Dispositivos Gravados</h6>
        </template>
        <b-collapse visible id="collapse-grav">
          <b-list-group>
            <b-list-group-item
              v-for="dev in bluetooth.devices.saved"
              v-bind:key="dev.address"
              class="flex-column align-items-start"
              @click="connect(dev)"
            >
              <div class="d-flex w-100 justify-content-between">
                <div class="mb-1">{{ dev.name }}</div>
                <small>{{ dev.address }}</small>
              </div>
            </b-list-group-item>
          </b-list-group>
        </b-collapse>
      </b-card>
      <b-card no-body header-tag="header">
        <template v-slot:header>
          <h6 class="mb-0" v-b-toggle.collapse-unpair>Dispositivos Descobertos</h6>
        </template>
        <b-collapse visible id="collapse-unpair">
          <b-list-group>
            <b-list-group-item
              v-for="dev in bluetooth.devices.unpaired"
              v-bind:key="dev.address"
              class="flex-column align-items-start"
            >
              <div class="d-flex w-100 justify-content-between">
                <div class="mb-1">{{ dev.name? dev.name:"Sem Nome" }}</div>
                <small>{{ dev.address }}</small>
              </div>
            </b-list-group-item>
          </b-list-group>
        </b-collapse>
      </b-card>
      <b-button variant="light" size="sm" @click="refreshDevices">Refresh</b-button>
    </b-card-group>
  </div>
</template>

<script>
// @ is an alias to /src
import { mapState, mapMutations } from "vuex";
// import HelloWorld from "@/components/HelloWorld.vue";

export default {
  name: "Home",
  // components: {
  //   HelloWorld
  // },
  data: function() {
    return {
      spinnerShow: false
    };
  },
  mounted() {
    console.log(window.cordova);
    if (window.cordova) {
      bluetoothSerial.enable(this.refreshDevices, this.errorEnable);
    }
  },
  computed: {
    ...mapState(["bluetooth"])
  },
  methods: {
    ...mapMutations(["setDevicesSaved", "setDevicesUnpaired", "setState"]),
    refreshDevices() {
      bluetoothSerial.list(this.setDevicesSaved, this.errorList);
      this.setEstado("Looking for Bluetooth Devices", 1);
      bluetoothSerial.discoverUnpaired(this.discUnpaired, this.errorUnpaired);
    },
    discUnpaired(dev) {
      this.setDevicesUnpaired(dev);
      this.setEstado("", 0);
    },
    setEstado(texto, spinner) {
      this.setState(texto);
      this.spinnerShow = spinner;
    },
    connect(dev) {
      console.log(dev);
      this.setEstado("Connecting", 1);
      bluetoothSerial.connect(dev.address, this.onconnect, this.ondisconnect);
    },
    onconnect: function() {
      this.setEstado("Connected.", 0);
    },
    ondisconnect: function() {
      this.setEstado("Disconnected.", 0);
    },
    errorEnable() {
      console.log("The user did *not* enable Bluetooth");
    },
    errorList() {
      console.log("errorList");
    },
    errorUnpaired() {
      console.log("errorUnpaired");
    }
  }
};
</script>
