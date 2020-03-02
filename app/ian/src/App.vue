<template>
	<b-col cols="12">
		<b-row>
			<b-col cols="12" class="mb-4 p-0">
				<b-navbar class="p-0" style="background: rgb(55, 147, 160); color: white;">
					<b-navbar-nav>
						<router-link to="/" class="mr-1">Connection</router-link>
						<router-link to="/about">Configuration</router-link>
					</b-navbar-nav>
				</b-navbar>
			</b-col>
		</b-row>
		<b-row>
			<router-view />
		</b-row>
	</b-col>
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

li a {
	color: #17a2b8 !important;
}

li a.active {
	color: #495057 !important;
}

.navbar-nav a {
	padding: 8px 8px !important;
	text-transform: uppercase;
	letter-spacing: 0.5px;
	font-size: 12px;
}

.router-link-exact-active.router-link-active {
	background: rgba(44, 42, 42, 0.44);
	padding: 8px 8px !important;
}

:focus {
	-webkit-box-shadow: none !important;
	box-shadow: none !important;
	outline: none !important;
}

label {
	margin-bottom: 0px !important;
}

.card {
	border: none !important;
}
</style>
