<template>
  <b-col cols="12">
    <b-card class="px-0">
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

      <b-col cols="12" class="px-0">
        <b-row v-if="spinnerShow">
          <b-col cols="12" style="text-align: right;">
            <h6>
              <b-badge variant="light" style="background: none;">
                {{bluetooth.state}}
                <b-spinner small></b-spinner>
              </b-badge>
            </h6>
          </b-col>
        </b-row>
        <b-row>
          <b-col cols="12" md="6" class="mb-1">
            <b-card class="cardNoPad" no-body header-tag="header" align="center">
              <b-card-header
                header-bg-variant="info"
                header-text-variant="white"
                v-b-toggle.collapse-grav
                header="Dispositivos Gravados"
              />
              <b-collapse visible id="collapse-grav">
                <b-list-group>
                  <b-list-group-item
                    v-for="dev in bluetooth.devices.saved"
                    v-bind:key="dev.address"
                    class="flex-column align-items-start"
                    @click="connect(dev)"
                  >
                    <div class="d-flex w-100 justify-content-between">
                      <label>{{ dev.name }}</label>
                      <label>
                        <small>{{ dev.address }}</small>
                      </label>
                    </div>
                  </b-list-group-item>
                </b-list-group>
              </b-collapse>
            </b-card>
          </b-col>
          <b-col cols="12" md="6" class="mb-1">
            <b-card no-body header-tag="header" align="center">
              <b-card-header
                header-bg-variant="info"
                header-text-variant="white"
                v-b-toggle.collapse-unpair
                header="Dispositivos Descobertos"
              />
              <b-collapse visible id="collapse-unpair">
                <b-list-group>
                  <b-list-group-item
                    v-for="dev in bluetooth.devices.unpaired"
                    v-bind:key="dev.address"
                    class="flex-column align-items-start"
                  >
                    <div class="d-flex w-100 justify-content-between">
                      <label>{{ dev.name? dev.name:"Sem Nome" }}</label>
                      <label>
                        <small>{{ dev.address }}</small>
                      </label>
                    </div>
                  </b-list-group-item>
                </b-list-group>
              </b-collapse>
            </b-card>
          </b-col>
        </b-row>
        <b-row class="mt-3" style="text-align: center;">
          <b-col cols="12">
            <b-button variant="danger" size="sm" @click="refreshDevices" style="width: 100%">
              <b-icon icon="arrowRepeat"></b-icon>&nbsp;Refresh
            </b-button>
          </b-col>
        </b-row>
      </b-col>
    </b-card>
  </b-col>
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
<style>
.cardNoPad .card-body {
  padding: 0px !important;
  border: 0px !important;
  border-radius: 0px;
}

.card label {
  font-size: 14px;
  font-weight: 600;
}

.card-header {
  padding-top: 2px;
  padding-bottom: 2px;
}
</style>
