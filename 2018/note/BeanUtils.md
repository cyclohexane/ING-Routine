package utils;

import java.lang.reflect.*;
import java.util.Map;

public class BeanUtils {

	public static void populate(Object bean, Map<String, String[]> map) throws Exception {
		Class<? extends Object> cls = bean.getClass();
		for (Field prop : cls.getDeclaredFields()) {// 获取bean中的所有成员变量（属性）并遍历
			String propName = prop.getName();// 获取某个属性的名称
			String[] propValue = map.get(propName);// 获取map中对应的属性值
			if (propValue == null) continue;
			String setterName = "set" + propName.substring(0, 1).toUpperCase() + propName.substring(1);
			Method setter = cls.getDeclaredMethod(setterName, new Class[] { prop.getType() });// 根据方法名和数据类型生成setter
			setter.invoke(bean, propValue[0]);// propValue是个字符串数组，这里取数组中第一个元素，否则会类型不匹配
		}
	}
}