import { ComponentFixture, TestBed } from '@angular/core/testing';

import { GraphAppComponent } from './graph-app.component';

describe('GraphAppComponent', () => {
  let component: GraphAppComponent;
  let fixture: ComponentFixture<GraphAppComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ GraphAppComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(GraphAppComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
