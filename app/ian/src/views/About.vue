<template>
	<b-row class="about px-4">
		<b-col cols="12">
			<b-card>
				<b-tabs content-class="mt-3">
					<b-tab title="Patterns" active>
						<b-row class="mb-2" style="text-align: left;">
							<b-col cols="12">
								<b-button style="background: #3793a0;" size="sm" class="mr-1">Círculo</b-button>
								<b-button style="background: #3793a0;" size="sm" class="mr-1">Quadrado</b-button>
								<b-button style="background: #3793a0;" size="sm">Triângulo</b-button>
							</b-col>
						</b-row>
						<b-row class="mb-2">
							<b-col cols="8" class="pr-0">
								<b-form-select size="sm"></b-form-select>
							</b-col>
							<b-col cols="4">
								<b-form-input size="sm"></b-form-input>
							</b-col>
						</b-row>
						<b-row class="mb-2">
							<b-col cols="12">
								<b-form-select size="sm"></b-form-select>
							</b-col>
						</b-row>
						<b-row class="mb-2" style="text-align: left;">
							<b-col cols="12">
								<b-button style="background: #3793a0;" size="sm" class="mr-1">Centro Fora</b-button>
								<b-button style="background: #3793a0;" size="sm" class="mr-1">Centro Dentro</b-button>
							</b-col>
						</b-row>
						<b-row class="mb-2 px-3">
							<b-col cols="12" style="border: 1px solid #3793a0; border-radius: 5px; height: 150px;">
								O Canva vem para aqui
							</b-col>
						</b-row>
						<b-row style="text-align: right;">
							<b-col cols="12">
								<b-button style="background: #3793a0;" size="sm">SET</b-button>
							</b-col>
						</b-row>
					</b-tab>
					<b-tab title="Text">
						<b-row class="mb-2">
							<b-col cols="12">
								<b-form-select size="sm"></b-form-select>
							</b-col>
						</b-row>
						<b-row style="text-align: left;">
							<b-col cols="12">
								 <b-form-checkbox switch size="sm">Centrado</b-form-checkbox>
							</b-col>
						</b-row>
					</b-tab>
				</b-tabs>
			</b-card>
		</b-col>
	</b-row>
</template>
