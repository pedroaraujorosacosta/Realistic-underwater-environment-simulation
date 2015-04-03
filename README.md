# thesiamesedream

Vou colocando aqui coisas 'a medida que reparo que estavam em falta na configuracao.

Esta solucao assume que as vars:
-OGRE_HOME
-OGRE_SAMPLES
estao definidas.

Qd fazemos build do ogre, ele cria uma pasta chamada build\sdk\. Essa pasta e' o que chamo o OGRE_HOME.
Outra pasta que ele cria e' a build\Samples\. Essa e' a OGRE_SAMPLES a que me refiro nos settings do projecto.

Isto e' mais para demorar menos tempo a recompilar coisas:
Left-click no projecto, escolham Properties -> C/C++ -> Precompiled Headers-> Precompiled header, escolham 'Use'.
Left-click no stdafx.cpp, escolham Properties -> C/C++ -> Precompiled Headers-> Precompiled header, escolham 'Create'.


Also, coloquem o ficheiro *.dat (que o gerador de terreno cria na pasta bin da sdk ao lado do executavel) na pasta de media apontada pelo ficheiro de recursos. Senao o gerador de terreno julga sempre que tem de regerar o terreno.

QQ coisa, digam.
