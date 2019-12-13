
#ifndef genNamedObjTemplate_H 
#define genNamedObjTemplate_H


/*
 * Template genNamedObj:
 *-----------------------
 *
 * This template encapsulates the handling of named object.
 * A named object is an object that has as identifier a string.
 * The objects are put into an STL map, can be accessed or iterated
 * over using the member functions begin(), end(), next().
 *
 * CW Feb 10, 1999
 *
 */


#include <algo.h>
#include <list.h>
#include <map.h>




static inline bool cstring_less_than(const char *s1, const char *s2)
{
  return strcmp(s1, s2) < 0 ;
}

static inline bool cstring_greater_than(const char *s1, const char *s2)
{
  return strcmp(s1,s2) > 0 ;
}

typedef pointer_to_binary_function<const char*, const char*, bool> cstring_ordering_t;


// why can't we put this into the template class????
static cstring_ordering_t my_cstring_ordering( cstring_less_than );






/**
 Template for genNamedObj.

 This template supports the store, retrieval and location of named objects.
 A named object is simply an object that has a name (in the form of a string).
 Internally it is implemented using the STL map container.

 {\bf Typical usage: }
 \begin{verbatim}
 class XXX : public genNamedObj<X*>
 {
   XXX(const char *name, ...) : getNamedObj<X*>( name, this ) {...}
 };

 // initialize the static members
 map<const char*, X*, cstring_ordering_t> genNamedObj<XXX*>::m_Map( my_cstring_ordering );
 map<const char*, XXX*, cstring_ordering_t>::iterator genNamedObj<XXX*>::m_It = NULL;

\end{verbatim}

 Note that we do normally not store the object itself in the container, but only
 the pointer to it. The string "name" is stored and managed internally.
 As the container is implemented as a static map, the user has to initialize
 its two static members, 

 An working example C++ file is test_gen_named_obj.cc.

 {\bf MODIFICATIONS: }
 \\

 @author C.Witzig \\
 {\bf Date: } Mar 2, 99  \\

 @version Last update May 2, 99
 */


template< class X >
class genNamedObj
{
 public:

  /** Constructor that add the named object x into the container.
      @param name: name of the object. A local copy of the name is made and maintained
      in the genNamedObj.
      @param X: the object. Normally the pointer to the object is passed and not the
      object itself in order to limit the number of copy Ctors
  */
  genNamedObj( const char *name, X x) { add( name, x ); }
	
  /**  Dtor. Removes the object from the container and deletes the name */
  ~genNamedObj() { remove( m_Name ); delete [] m_Name; }

  /** Returns the name of the object */
  inline const char *getObjName() const { return m_Name; }
  
  /** Returns true of the object with this name exists */
  static bool exists( const char *name );

  /**  Returns the object of a given name or NULL if it does not exists */
  static X get( const char *name ) { return exists( name ) ? m_Map[name] : NULL; }
  
  /** Returns the object of a given name or NULL if it does not exists. 
      This function is only provided as the "generic" name get may already be
      used by another object leading to name conflicts. In that case we can
      use getObject and get will not be instantiated by the compiler from
      the template.
  */
  static X getObject( const char *name ) { return exists( name ) ? m_Map[name] : NULL; }

  /** Given a tag (such as "object*") find all objects that contain this string and
      return them in a linked list. The return value is the number of objects in
      the list.
  */
  static int getObjects( const char *name, list<X> &theList );
  
  
  /** Returns the location of the object within the map - i.e. for the n-th object returns n */
  int getLocation();
  
  /** Returns the number of objects in the container */
  inline static int getNbElements() { return m_Map.size(); }
  
  /** Adds the object X with the name to the container. Is normally not called by the user
      but from within the constructor.
  */
  static void add( const char *name, X x );

  /** Removes the named object from the list. Is normally not called by the user but
      from the Dtor.
  */
  static void remove( const char *name );
  
  /**  Returns the first object  */
  inline static X begin() { m_It = m_Map.begin(); return (*m_It).second; }

  /**  Returns the position after the last object (i.e. NULL) */
  inline static X end() { return NULL; }
  
  /**  Returns the next object */
  inline static X next() { m_It++; return m_It != m_Map.end() ? (*m_It).second : NULL; }


 private:
  
  /**  Name of the object */
  char *m_Name;

  /**  map with all the elements */
  static map< const char*, X, cstring_ordering_t > m_Map;

  /** iterator that is used internally when looping over all objects */
  static map< const char*, X, cstring_ordering_t >::iterator m_It;

};




template <class X >
void genNamedObj<X>::add( const char* name, X x )
{
  if ( exists(name ) == 0 ) {
    x->m_Name = new char[ strlen(name) + 1];
    strcpy( x->m_Name, name );
    m_Map[ x->m_Name ] = x;
  }
}


template <class X>
void genNamedObj<X>::remove( const char *name )
{
  map<const char*, X, cstring_ordering_t>::iterator it;
  for ( it = m_Map.begin(); it != m_Map.end(); it++ ) {
    if ( ! strcmp((*it).second->getObjName(), name ) )
      break;
  }

  if ( it != m_Map.end() ) {
    m_Map.erase( it );
  }
}



template <class X >
bool genNamedObj<X>::exists( const char *name )
{
  map<const char*, X, cstring_ordering_t>::iterator it;
  for ( it = m_Map.begin(); it != m_Map.end(); it++ ) {
    if ( ! strcmp( (*it).second->m_Name, name ) )
      break;
  }

  return ( it != m_Map.end() ) ? true : false;
}



template <class X >
int genNamedObj<X>::getLocation()
{
  int iLocation = 0;

  map<const char*, X, cstring_ordering_t>::iterator it;
  for ( it = m_Map.begin(); it != m_Map.end(); it++ ) {
    if ( ! strcmp( (*it).second->m_Name, m_Name ) ) 
      break;
    iLocation++;
  }

  return iLocation;
}



template <class X>
int genNamedObj<X>::getObjects( const char *pcString, list<X> &theList )
{
  char *pcNameTag = new char[strlen( pcString ) + 1];
  if ( pcString[0] == '*' )
    strcpy( pcNameTag, &pcString[1] );
  else
    strcpy( pcNameTag, pcString );

  char *pc;
  if ( ( pc = strchr( pcNameTag, '*' ) ) != NULL )
    *pc = '\0';


  map<const char*, X, cstring_ordering_t>::iterator it;
  for ( it = m_Map.begin(); it != m_Map.end(); it++ ) {
    if ( strstr( (*it).second->m_Name, pcNameTag ) )
      theList.push_back( (*it).second );
  }

  return theList.size();
}
    

#endif

