#version 440 core

struct Material 
{
    vec3 ambientColor;
    float ambientStrength;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
}; 

struct LightSource 
{
    vec3 position;	
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float focalStrength;
    float specularIntensity;
};

#define TOTAL_LIGHTS 4

in vec3 fragmentPosition;
in vec3 fragmentVertexNormal;
in vec2 fragmentTextureCoordinate;

out vec4 outFragmentColor;

uniform bool bUseTexture=false;
uniform bool bUseTextureOverlay=false;
uniform bool bUseLighting=false;
uniform vec4 objectColor = vec4(1.0f);
uniform sampler2D objectTexture;
uniform sampler2D objectTextureOverlay;
uniform vec3 viewPosition;
uniform vec2 UVscale = vec2(1.0f, 1.0f);
uniform LightSource lightSources[TOTAL_LIGHTS];
uniform Material material;

// function prototypes
vec3 CalcLightSource(LightSource light, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection);

void main()
{
   vec4 effectiveObjectColor;
   
   if ((bUseTexture == false) && (bUseTextureOverlay == true))
   {
		vec4 textureOverlayColor = texture(objectTextureOverlay, fragmentTextureCoordinate * UVscale);
		effectiveObjectColor = mix(objectColor, textureOverlayColor, 0.5);
   }
   else {
      effectiveObjectColor = objectColor;
   }

   if(bUseLighting == true)
   {
      // properties
      vec3 lightNormal = normalize(fragmentVertexNormal);
      vec3 viewDirection = normalize(viewPosition - fragmentPosition);
      vec3 phongResult = vec3(0.0f);

      for(int i = 0; i < TOTAL_LIGHTS; i++)
      {
         phongResult += CalcLightSource(lightSources[i], lightNormal, fragmentPosition, viewDirection); 
      }   
    
      if(bUseTexture == true)
      {
         vec4 textureColor = texture(objectTexture, fragmentTextureCoordinate * UVscale);
		 if (bUseTextureOverlay)
		 {
		    vec4 textureOverlayColor = texture(objectTextureOverlay, fragmentTextureCoordinate * UVscale);
			textureColor = mix(textureColor, textureOverlayColor, 0.5);
		 }
         outFragmentColor = vec4(phongResult * textureColor.xyz, 1.0);
      }
      else
      {
         outFragmentColor = vec4(phongResult * effectiveObjectColor.xyz, objectColor.w);
      }
   }
   else 
   {
      if(bUseTexture == true)
      {
         outFragmentColor = texture(objectTexture, fragmentTextureCoordinate * UVscale);
      }
      else
      {
         outFragmentColor = effectiveObjectColor;
      }
   }
   
   if (bUseTextureOverlay)
   {
      
   }
}

// calculates the color when using a directional light.
vec3 CalcLightSource(LightSource light, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection)
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   //**Calculate Ambient lighting**

   ambient = light.ambientColor + (material.ambientColor * material.ambientStrength);

   //**Calculate Diffuse lighting**

   // Calculate distance (light direction) between light source and fragments/pixels
   vec3 lightDirection = normalize(light.position - vertexPosition); 
   // Calculate diffuse impact by generating dot product of normal and light
   float impact = max(dot(lightNormal, lightDirection), 0.0);
   // Generate diffuse material color   
   diffuse = impact * material.diffuseColor; 

   //**Calculate Specular lighting**

   // Calculate reflection vector
   vec3 reflectDir = reflect(-lightDirection, lightNormal);
   // Calculate specular component
   float specularComponent = pow(max(dot(viewDirection, reflectDir), 0.0), light.focalStrength);
   specular = (light.specularIntensity * material.shininess) * specularComponent * material.specularColor;
  
   return(ambient + diffuse + specular);
}