<template>
	<div id="app">
		<b-row>
			<b-col cols="12" class="mb-4">
				<b-navbar type="dark" variant="info">
					<b-navbar-nav>
						<router-link to="/">
							<b-button variant="light" size="sm" class="mr-2">
								<b>Connection</b>
							</b-button>
						</router-link>
						<router-link to="/about">
							<b-button size="sm" variant="light">
								<b>Configuration</b>
							</b-button>
						</router-link>
					</b-navbar-nav>
				</b-navbar>
			</b-col>
		</b-row>
		<router-view/>
	</div>
</template>

<style lang="scss">
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

#nav {
  padding: 30px;

  a {
	font-weight: bold;
	color: #2c3e50;

	&.router-link-exact-active {
	  color: #3793a0;
	}
  }
}

.navbar button {
  text-transform: uppercase;
  opacity: 1;
}

</style>
