<template>
	<div id="app">
		<b-row>
			<b-col cols="12" class="mb-4">
				<b-navbar style="background: rgb(242, 206, 5); color: white;">
					<b-navbar-nav>
						<router-link to="/" class="mr-1">
							<b>Connection</b> |&nbsp;
						</router-link>
						<router-link to="/about">
							&nbsp;<b>Configuration</b>
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

.navbar a {
	color: white;
	text-decoration: none;
}

.navbar button {
  text-transform: uppercase;
  opacity: 1;
}

</style>
