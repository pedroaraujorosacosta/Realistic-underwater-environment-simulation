# thesiamesedream

Ja sei que me vai faltar aqui qq coisa, mas pronto.

Esta solucao assume que as vars:
-OGRE_HOME
-OGRE_SAMPLES
estao definidas.

Qd fazemos build do ogre, ele cria uma pasta chamada build\sdk\. Essa pasta e' o que chamo o OGRE_HOME.
Outra pasta que ele cria e' a build\Samples\. Essa e' a OGRE_SAMPLES a que me refiro nos settings do projecto.

Compilem uma vez. Depois de compilarem uma vez, vao aos settings do projecto->c/c++->Precompiled Headers->Precompiled Header->Use.
Isto e' mais para demorar menos tempo a recompilar coisas.

Also, coloquem o ficheiro *.dat (que o gerador de terreno cria na pasta bin da sdk ao lado do executavel) na pasta de media apontada pelo ficheiro de recursos. Senao o gerador de terreno julga sempre que tem de regerar o terreno.

QQ coisa, digam.
