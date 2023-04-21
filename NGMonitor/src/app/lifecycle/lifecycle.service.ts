import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { catchError, retry } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable()
export class LifecycleService {

    constructor(private http: HttpClient) { }

    SERVER_ADRESS = '/api/';
    LIFECYCLE = 'lifecycle';

    getOfferedServices(): Observable<any> {
        return this.http.get(`${this.SERVER_ADRESS}${this.LIFECYCLE}`).pipe(
            retry(3),
            catchError(this.handleError)
        );
            
    }

    private handleError(error: any) {
        const errMsg = (error.message) ? error.message :
            error.status ? `${error.status} - ${error.statusText}` : 'Server error';
        return errMsg;
    }
}