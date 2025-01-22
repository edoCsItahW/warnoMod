/**
 * @file utils.ts
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/19 下午12:45
 * @desc
 * */

export function isDigit(str: string): boolean {
    return /^\d+$/.test(str);
}

export function isAlpha(str: string): boolean {
    return /^[a-zA-Z]+$/.test(str);
}

export function isSpace(str: string): boolean {
    return /^\s+$/.test(str);
}

export function enumToStr<T>(e: T, v: number): string {
    return (e as any)[v];
}
