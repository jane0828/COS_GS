
// #include <sgs/frontend/model.h>

// extern StateCheckUnit State;
// extern Console console;

// Shader * ShaderFootprint;
// Spline * Footprint;


// GLint TextureFromFile( const char *path, std::string directory )
// {
//     //Generate texture ID and load texture data
//     std::string filename = std::string( path );
//     filename = directory + '/' + filename;
//     GLuint textureID;
//     glGenTextures( 1, &textureID );

//     int width, height, nrChannels;
//     unsigned char *image = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
//     if(State.Debugmode)
//     {
//         if (!image) {
//         printf("texture %s loading error ... \n", filename.c_str());
//         }
//         else printf("texture %s loaded\n", filename.c_str());
//     }
    

//     GLenum format;
//     if (nrChannels == 1) format = GL_RED;
//     else if (nrChannels == 3) format = GL_RGB;
//     else if (nrChannels == 4) format = GL_RGBA;

//     glBindTexture(GL_TEXTURE_2D, textureID);
//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // OpenGL requires a texture image's dimension should be divided by 4. This function solve the problem

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
//     glGenerateMipmap(GL_TEXTURE_2D);

//     stbi_image_free(image);

//     return textureID;
// }

// double SetLatitudeScale(SatelliteObject * Sat)
// {
//     double R = 6371.0f;
//     double theta = acos(R / (Sat->geo.altitude + R));
//     return theta;
// }


// void SatelliteModelInitialize(SatelliteObject * Sat)
// {
//     Sat->Satellites2D = new Rectangle(false);
//     Sat->ShaderSatellites2D = new Shader("./src/shader/3DTexture.vs", "./src/shader/3DTexture.fs", NULL);
//     Sat->Satellites2D->XYZScale(0.02, 0.02, 0.0);
//     Sat->SatRange2D = new Circle();
//     Sat->ShaderSatRange2D = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
//     Sat->Satpoint = new Circle();
//     Sat->ShaderSatpoint = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
// }

// void SatelliteModelDelete(SatelliteObject * Sat)
// {
//     delete Sat->Satellites2D;
//     delete Sat->ShaderSatellites2D;
//     delete Sat->SatRange2D;
//     delete Sat->ShaderSatRange2D;
//     delete Sat->Satpoint;
//     delete Sat->ShaderSatpoint;
//     Sat->Satellites2D = NULL;
//     Sat->ShaderSatellites2D = NULL;
//     Sat->SatRange2D = NULL;
//     Sat->ShaderSatRange2D = NULL;
//     Sat->Satpoint = NULL;
//     Sat->ShaderSatpoint = NULL;
// }



// void SatDisplayer(SatelliteObject * Sat, GLint * SatTexture)
// {
//     if(State.TLEnow)
//     {
//         // if(State.Satellites == NULL)
//         //     break;
//         // if(State.Satellites->use == false)
//         //     continue;

//         Sat->ShaderSatRange2D->use();
//         // SatRange2D->Setradius(0.5f);
//         Sat->SatRange2D->Setradius((float)SetLatitudeScale(Sat) / (M_PI / 2));
//         Sat->SatRange2D->Mercator((float)Sat->geo.latitude, (float)Sat->geo.longitude);
//         Sat->SatRange2D->Setcolor(1.0f, 1.0f, 1.0f);
//         Sat->SatRange2D->Update();
//         Sat->ShaderSatRange2D->setFloat("alpha", 0.25f);
//         Sat->SatRange2D->drawFan(Sat->ShaderSatRange2D);
//         Sat->SatRange2D->Setcolor(1.0f, 1.0f, 0.0f);
//         Sat->SatRange2D->Update();
//         Sat->ShaderSatRange2D->setFloat("alpha", 1.0f);
//         Sat->SatRange2D->drawLoop(Sat->ShaderSatRange2D);
        
//         Sat->Satellites2D->XYTranslate(Sat->DisplayPoint_Longitude(), Sat->DisplayPoint_Latitude());
//         Sat->ShaderSatellites2D->use();
//         glActiveTexture(GL_TEXTURE0 + 2);
//         glBindTexture(GL_TEXTURE_2D, *SatTexture);
//         Sat->ShaderSatellites2D->setInt("texture1", 2);
//         Sat->Satellites2D->Draw(Sat->ShaderSatellites2D);
            
//     }
// }

// void SatpointDisplayer(SatelliteObject * Sat)
// {
//     if(State.TLEnow)
//     {
//         // if(State.Satellites == NULL)
//         //     break;
//         // if(State.Satellites->use == false)
//         //     continue;
//         // if(State.Satellites->topo.elevation < 0.0f)
//         //     continue;
//         Sat->Satpoint->Setradius(0.02f);
//         Sat->Satpoint->Setcolor(1.000f, 1.000f, 0.000f);
//         Sat->Satpoint->translate(0.9f * (float)(sin(Sat->topo.azimuth) * (M_PI / 2 - Sat->topo.elevation) / (M_PI / 2)), 0.9f * (float)(cos(Sat->topo.azimuth) * (M_PI / 2 - Sat->topo.elevation) / (M_PI / 2)), 0);
//         Sat->Satpoint->Update();
//         Sat->ShaderSatpoint->use();
//         Sat->ShaderSatpoint->setFloat("alpha", 1.0f);
//         Sat->Satpoint->drawLoop(Sat->ShaderSatpoint);
//     }
    
// }

// void PathInitializer()
// {
//     PathDelete();
//     ShaderFootprint = new Shader("./src/shader/3DRGB.vs", "./src/shader/3DRGB.fs", NULL);
//     Footprint = new Spline();
// }

// void PathGenerator(SatelliteObject * Sat)
// {
//     Sat->GeneratePath();
//     if(State.TLEnow)
//     {
        
//         int k = 0;
//         for(int i = 0; i < 1024; i++)
//         {
//             Footprint->spline_vertices[k] = 0.9f * (float)(sin(Sat->path_az[i]) * (M_PI / 2 - Sat->path_el[i]) / (M_PI / 2));
//             Footprint->spline_vertices[k + 1] = 0.9f * (float)(cos(Sat->path_az[i]) * (M_PI / 2 - Sat->path_el[i]) / (M_PI / 2));
//             Footprint->spline_vertices[k + 2] = 0;

//             if(i >= 1)
//             {
//                 if(abs(Sat->path_az[i] - Sat->path_az[i - 1]) > 2.0f / RAD_TO_DEG || abs(Sat->path_el[i] - Sat->path_el[i - 1]) > 2.0f / RAD_TO_DEG)
//                 {
//                     Footprint->spline_vertices[k] = Footprint->spline_vertices[k - 3];
//                     Footprint->spline_vertices[k + 1] = Footprint->spline_vertices[k - 2];
//                     console.AddLog("[DEBUG]##PathFinder Error. Calculate again this spline vertices. index : %d, %d, %d", k, k + 1, k + 2);
//                     // printf("Az : %lf, El : %lf, Az_passed : %lf, El_passed : %lf\n", State.Satellites[Selected]->path_az[i], State.Satellites[Selected]->path_el[i], State.Satellites[Selected]->path_az[i-1], State.Satellites[Selected]->path_el[i-1]);
            
//                 }
//             }
//             k += 3;
            
//         }
//         Footprint->Setcolor(1.000f, 0.447f, 0.463f);
//     }
    
// }

// void PathDisplayer()
// {
//     if(State.TLEnow)
//     {
//         ShaderFootprint->use();
//         Footprint->Update();
//         ShaderFootprint->setFloat("alpha", 1.0f);
//         Footprint->draw(ShaderFootprint);
//     }
    
// }

// void PathDelete()
// {
//     delete ShaderFootprint;
//     ShaderFootprint = NULL;
//     delete Footprint;
//     Footprint = NULL;
// }