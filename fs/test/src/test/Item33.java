package test;

import java.lang.reflect.AnnotatedElement;
import java.text.Annotation;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class Item33 {
	public static void main(String[] args) {
		Favorites f = new Item33().new Favorites();
		Class aClass = Integer.class;
		f.putFavorite(String.class, "ddd");
		f.putFavorite(Annotation.class, null);
		f.putFavorite(Integer.class, 10);

		f.putFavorite(Integer.class, 0xcafebabe);
		f.putFavorite(Class.class, Favorites.class);
		
		Object favoriteString = f.getFavorite(Annotation.class);
		int favoriteInteger = f.getFavorite(Integer.class);
		Object favoriteClass = f.getFavorite(Integer.class);
		System.out.printf("%s %x %x%n", favoriteString,favoriteInteger, favoriteClass);
		
	}
	

	
	
	public class Favorites {
		private Map<Class<? extends Annotation>, Object> favorites = new HashMap<>();
		
		public <T> void putFavorite(Class<? extends Annotation> type, T instance) {
			favorites.put(Objects.requireNonNull(type), type.cast(instance));
		}
		public <T> T getFavorite(Class<T> type) {
			return type.cast(favorites.get(type));
		}
		
		Annotation getAnnotation(AnnotatedElement element, String annotationTypeName) {
			Class<?> annotationType = null;
			
			try {
				annotationType = Class.forName(annotationTypeName);
				} 
			catch (Exception ex) {throw new IllegalArgumentException(ex);}
			
			return element.getAnnotation(annotationType.asSubclass(Annotation.class));
			}
		}
	}
	
	

